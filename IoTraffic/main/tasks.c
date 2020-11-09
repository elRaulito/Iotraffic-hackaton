/*
 * tasks.c
 *
 *  Created on: 9 nov 2020
 *      Author: Raul Rosa
 */

#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "tasks.h"
#include "driver/gpio.h"
#include "fetch.h"
#include "string.h"
#include "cJSON.h"



extern int carsIota;
extern int carsTangle;
extern int accidents;
extern int flagConnected;
extern int dutyTraffic;


#define GREEN1  GPIO_NUM_4
#define RED1    GPIO_NUM_15
#define YELLOW1 GPIO_NUM_2
#define GREEN2  GPIO_NUM_16
#define RED2    GPIO_NUM_5
#define YELLOW2 GPIO_NUM_17


extern char channelNVS[105];
extern char gatewayNVS[30];
extern xSemaphoreHandle connectionSemaphore;



//This task will perform a comparison between cars of iota street and tangle road, and changing the timing of the traffic light according to the data

void taskComputing(void *params){

	while(1){


		if (flagConnected){
        if(carsIota>carsTangle){
        	dutyTraffic=1;
        }else{
         if(carsIota<carsTangle){
            	dutyTraffic=9;
          }
          else{
        	dutyTraffic=5;
          }
        }

		}
		if(accidents>0){//if accidents are in the street activate the power mosfet to turn on the ambulance
			gpio_set_level(GPIO_NUM_23, 1);//MOSFET
		}else{
			gpio_set_level(GPIO_NUM_23, 0);
		}

		printf("Iota Cars %d Tangle Cars %d DutyCycle= %d accidents %d\n",carsIota,carsTangle, dutyTraffic,accidents);
		vTaskDelay(1000/portTICK_PERIOD_MS);
	}


}


//the switching of the traffic light
void taskLight(void* params){
	while(1){
		gpio_set_level(RED2, 1);//RED2
		gpio_set_level(YELLOW2, 0);//YELLOW2
		vTaskDelay((500)/portTICK_PERIOD_MS);

		gpio_set_level(RED1, 0);//RED1
		gpio_set_level(GREEN1, 1);//GREEN1
		vTaskDelay((dutyTraffic*1000)/portTICK_PERIOD_MS);

		gpio_set_level(GREEN1, 0);//GREEN1
		gpio_set_level(YELLOW1, 1);//YELLOW1
		vTaskDelay((1000)/portTICK_PERIOD_MS);

		gpio_set_level(YELLOW1, 0);//YELLOW1
		gpio_set_level(RED1, 1);//RED1
		vTaskDelay((500)/portTICK_PERIOD_MS);

		gpio_set_level(GREEN2, 1);//GREEN2
		gpio_set_level(RED2, 0);//RED2
		 vTaskDelay(((10-dutyTraffic)*1000)/portTICK_PERIOD_MS);

		gpio_set_level(GREEN2, 0);//GREEN2
		gpio_set_level(YELLOW2, 1);//YELLOW2
		 vTaskDelay((1000)/portTICK_PERIOD_MS);
	}

}

//Convert the json data in integers numbers
void onGotData(char *data){

	int flagIota=0;
	int flagTangle=0;

	cJSON *payload= cJSON_Parse(data);

	int size=0;
	size=cJSON_GetArraySize(payload);
    printf("size %d\n",size);
    int i=0;
    for(i=0;i<size;i++){
    cJSON *singleData=cJSON_GetArrayItem(payload,i);
    cJSON *messages=cJSON_GetObjectItem(singleData,"message");
    cJSON *tangle=cJSON_GetObjectItem(messages,"iot2tangle");
    cJSON *dataStreet=cJSON_GetObjectItem(cJSON_GetArrayItem(tangle,0),"data");
    cJSON *name=cJSON_GetObjectItem(cJSON_GetArrayItem(dataStreet,0),"name");

    cJSON *dataCars=cJSON_GetObjectItem(cJSON_GetArrayItem(tangle,1),"data");
    cJSON *number=cJSON_GetObjectItem(cJSON_GetArrayItem(dataCars,0),"cars");
    cJSON *numberAccidents=cJSON_GetObjectItem(cJSON_GetArrayItem(dataCars,0),"accidents");

    if((strstr(name->valuestring,"Iota")!=NULL)&&(flagIota==0)){
    	carsIota=atoi(number->valuestring);
    	accidents=atoi(numberAccidents->valuestring);
    	flagIota=1;
    }

    if((strstr(name->valuestring,"Tangle")!=NULL)&&(flagTangle==0)){
    	carsTangle=atoi(number->valuestring);
    	accidents=atoi(numberAccidents->valuestring);
    	flagTangle=1;
    }

    if((flagIota==1)&&(flagTangle==1)){
    	break;
    }

    }

   cJSON_Delete(payload);




}

void getRestTask(void*params){

	while(1){

		if (xSemaphoreTake(connectionSemaphore,100/portTICK_PERIOD_MS)){
			 struct  FetchParams TangleREST;
			 TangleREST.headerCount=2;
			 TangleREST.onGotData=onGotData;
			 TangleREST.method=GET;


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

			 TangleREST.header[0]=hostTangleHeader;
			 TangleREST.header[1]=hostTypeHeader;
			 TangleREST.body=NULL;

			 char request[500];
			 strcpy(request,"http://");
			 strcat(request,gatewayHost);
			 strcat(request,"/messages?channelId=");//let's validate the messages with the channel
			 strcat(request,channelNVS);
			 strcat(request,"&limit=5");//just the last 5 messages
			 printf("richiesta = %s\n",request);
			 fetch(request,&TangleREST);
			 esp_wifi_disconnect();
		 }

		vTaskDelay(5000/portTICK_PERIOD_MS);
	}

}
