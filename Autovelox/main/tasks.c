/*
 * tasks.c
 *
 *  Created on: 11 nov 2020
 *      Author: Raul Rosa
 */

#include "fetch.h"
#include "string.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "camera.h"
#include "esp_wifi.h"
#include "driver/gpio.h"

//GPIO PIN TO ULTRASONIC SENSOR
#define TRIGGER 12
#define ECHO 2

//FLASHLED GPIO
#define FLASHLED 4

extern char gatewayNVS[20],deviceidNVS[20];
extern float speed,fine,speedLimit;
char startTangleBody[]="{ \"iot2tangle\": [ { \"sensor\":\"Address\", \"data\": [ { \"name\": \"I got you street\" } ] } ,{\"sensor\": \"Camera\",\"data\":[{\"photo\":\"";
char midTangleBody[]=" euro\"}]}], \"device\": \"";
char endTangleBody[]="\", \"timestamp\": 0  }";

extern uint8_t *buffer;

extern xSemaphoreHandle connectionSemaphore;
extern xSemaphoreHandle speedSemaphore;


float distance;
float actualValue,previousValue;

float filter[10];
float distanceFiltered;

void addToFilter(float input){
	int i;
	float sum=0;
	for(i=0;i<10;i++){
		if(i==9){
			filter[i]=input;
		}else{
			filter[i]=filter[i+1];
		}
		sum+=filter[i];
	}
	distanceFiltered=sum/10;
	return;
}

void UltrasonicTask(void* params){
	while(1){
	//send a pulse of 10 us to activate the ultrasonic sensor
	gpio_set_level(TRIGGER, 1);
	ets_delay_us(10);//10us di giu
	gpio_set_level(TRIGGER, 0);

	//let's count the time till we get back the pulse
	int64_t begin = esp_timer_get_time();
	int64_t timeout;
	while(!gpio_get_level(ECHO)){
		timeout=esp_timer_get_time()-begin;
		if(timeout>100000){
			printf("first timeout %lld\n",timeout);//if the sensor gets stucked
			break;
		}
	}

	int64_t start= esp_timer_get_time();
	begin = esp_timer_get_time();
	while(gpio_get_level(ECHO)){
		timeout=esp_timer_get_time()-begin;
		if(timeout>100000){
			printf("second timeout\n");//if the pulse does not come back we must exit
			break;
		}
	}
	int64_t end= esp_timer_get_time();


	distance=(((float)(end-start))/58); //we can compute the distance in cm


	addToFilter(distance);//we use a mean average filter to reduce spikes

	if(distanceFiltered<=25){//if distance is larger than 25 cm we skip it
		previousValue=actualValue;
		actualValue=distanceFiltered;
	}

	if((previousValue-actualValue)>0){//if the object is coming closer to the sensor we record the data

		//let's compute the speed
		speed=previousValue-actualValue;
		speed=(speed*3.3);
		printf("value before %f, value now %f\n",previousValue,actualValue);
		printf("speed =%f\n\n",speed);

		if(speed>=speedLimit){
		fine=10*speed;
    		printf("fine=%f\n",fine);

		//activate the flashlight
		xSemaphoreGive(speedSemaphore);
		vTaskDelay(5000/portTICK_PERIOD_MS);//no speed is detected while sending the picture to the tangle
		}

	}else{
		speed=0;
	}

	vTaskDelay(300/portTICK_PERIOD_MS);
	}

}


void ShootTask(void *params){

	while(1){
        //only if speed limit is passed activate the flash
		if(xSemaphoreTake(speedSemaphore,1000/portTICK_PERIOD_MS)){


			gpio_set_level(FLASHLED, 1);
			vTaskDelay(300/portTICK_PERIOD_MS);
			gpio_set_level(FLASHLED, 0);

             //if wifi available it will send data to tangle
			if (xSemaphoreTake(connectionSemaphore, 1000/portTICK_PERIOD_MS)){
				 camera_capture();

				 //prepare the REST request
				 struct  FetchParams TangleParams;
				 TangleParams.headerCount=2;
				 TangleParams.onGotData=NULL;
				 TangleParams.method=POST;
				 char gatewayHost[30];
				 strcpy(gatewayHost,gatewayNVS);
				 Header hostTangleHeader={
					 .key="Host",
					 .value=gatewayHost
				 };
				 Header hostTypeHeader={
						 .key="Content-Type",
						 .value="application/json"
				 };

				 char BodyTangle[20000];
				 char request[100];
				 char speedChar[10];
				 char limitChar[10];
				 char fineChar[10];

				 sprintf(speedChar,"%.2f",speed);
				 sprintf(limitChar,"%.2f",speedLimit);
				 sprintf(fineChar,"%.2f",fine);

				 strcpy(BodyTangle,startTangleBody);
				 strcat(BodyTangle,(char*)buffer);
				 strcat(BodyTangle,"\",\"speed\":\"");
				 strcat(BodyTangle,speedChar);
				 strcat(BodyTangle," cm/s\",\"limit\":\"");
				 strcat(BodyTangle,limitChar);
				 strcat(BodyTangle," cm/s\",\"fine\":\"");
				 strcat(BodyTangle,fineChar);
				 strcat(BodyTangle,midTangleBody);
				 strcat(BodyTangle,deviceidNVS);
				 strcat(BodyTangle,endTangleBody);

				 TangleParams.header[0]=hostTangleHeader;
				 TangleParams.header[1]=hostTypeHeader;
				 TangleParams.body=BodyTangle;
				 strcat(gatewayHost,"/messages");

				 strcpy(request,"http://");
				 strcat(request,gatewayHost);
				 fetch(request,&TangleParams);



				 esp_wifi_disconnect();//disconnect from wifi, if not done sometimes after many calls REST calls will be blocked
				 free(buffer);


			}


		}



		vTaskDelay(500/portTICK_PERIOD_MS);
	}

}
