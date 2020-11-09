#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "connect.h"
#include "esp_log.h"
#include "fetch.h"
#include "string.h"
#include "cJSON.h"
#include "server.h"
#include "tasks.h"

#define TAG "server"
xSemaphoreHandle connectionSemaphore;

int dutyTraffic=5;//range 1-9, 5 means that each green light will last for 5 seconds
int accidents;

//the number of cars in iota street and tangle road
int carsIota=0;
int carsTangle=0;

int flagConnected=0;//flag used to detect if connection is available
char channelNVS[105]; //105 is the size of the channel string
char gatewayNVS[30];  //it will be improved using malloc instead of fixed length array

static esp_err_t event_handler(void *ctx, system_event_t *event)
{

	  switch (event->event_id)
	  {
	  case SYSTEM_EVENT_STA_START://in case wifi does not connect try again
	    if(esp_wifi_connect()!=ESP_OK){
	    	esp_wifi_stop();
	    	connectSTA();

	    }else{
	    ESP_LOGI(TAG,"connecting...\n");
	    }
	    break;

	  case SYSTEM_EVENT_STA_CONNECTED:
	    ESP_LOGI(TAG,"connected\n");
	    break;

	  case SYSTEM_EVENT_STA_GOT_IP:
	    ESP_LOGI(TAG,"got ip\n");
	    xSemaphoreGive(connectionSemaphore);//after esp32 gets IP address we can start REST calls
	    flagConnected=1;

	    break;

	  case SYSTEM_EVENT_STA_DISCONNECTED: //in case of disconnection remove the semaphore and try to reconnect again
		if(xSemaphoreTake(connectionSemaphore, 1000/portTICK_PERIOD_MS)){
			printf("semaphore removed\n");
		}
		    flagConnected=0;
	    	esp_wifi_stop();
	    	connectSTA();
	    break;

	  case SYSTEM_EVENT_AP_STACONNECTED:


	  break;

	  case SYSTEM_EVENT_AP_STADISCONNECTED:

	  break;

	  case SYSTEM_EVENT_AP_STOP:
      break;

	  default:
	    break;
	  }
	  return ESP_OK;
}





void initGPIO(){

	gpio_pad_select_gpio(GPIO_NUM_15);
    gpio_pad_select_gpio(GPIO_NUM_23);

    gpio_set_direction(GPIO_NUM_4, GPIO_MODE_OUTPUT);//GREEN1
    gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);//YELLOW1
    gpio_set_direction(GPIO_NUM_15, GPIO_MODE_OUTPUT);//RED1

    gpio_set_direction(GPIO_NUM_16, GPIO_MODE_OUTPUT);//GREEN2
    gpio_set_direction(GPIO_NUM_17, GPIO_MODE_OUTPUT);//YELLOW2
    gpio_set_direction(GPIO_NUM_5, GPIO_MODE_OUTPUT);//RED2


    gpio_set_direction(GPIO_NUM_23, GPIO_MODE_OUTPUT); //MOSFET OUTPUT PIN
}



void app_main(void)
{
    nvs_flash_init();
    tcpip_adapter_init();
    ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
    initGPIO();

    //initialize tcpip protocol and start wifi
    tcpip_adapter_init();
    connectionSemaphore=xSemaphoreCreateBinary();
    wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_config));
    ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE));

    //create it's own wifi to configurate the board
    connectAP();
    RegisterEndPoints();//Register end points for dashboard



    //We will run 3 tasks in different cores, core 0 will be the traffic light while core 1 will  compute the timing and make the REST task
    xTaskCreatePinnedToCore(taskLight,"Traffic light task",1024*10,NULL,3,NULL,0);
    xTaskCreatePinnedToCore(taskComputing,"Compute timing task",1024*10,NULL,4,NULL,1);
    xTaskCreatePinnedToCore(getRestTask,"Rest task",1024*100,NULL,2,NULL,1);
}

