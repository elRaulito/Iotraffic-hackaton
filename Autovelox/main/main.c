#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include <esp_timer.h>
#include "esp_log.h"
#include "mbedtls/base64.h"
#include "tasks.h"

#include "esp_camera.h"
#include "connect.h"
#include "fetch.h"
#include "string.h"
#include "server.h"
#include "camera.h"




char gatewayNVS[20],deviceidNVS[20];
float speed,fine,speedLimit;


//GPIO PIN TO ULTRASONIC SENSOR
#define TRIGGER 12
#define ECHO 2

//FLASHLED GPIO
#define FLASHLED 4



xSemaphoreHandle connectionSemaphore;
xSemaphoreHandle speedSemaphore;

#define TAG "server"







static esp_err_t event_handler(void *ctx, system_event_t *event)
    {

    	  switch (event->event_id)
    	  {
    	  case SYSTEM_EVENT_STA_START:
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
    	    xSemaphoreGive(connectionSemaphore);

    	    break;

    	  case SYSTEM_EVENT_STA_DISCONNECTED:
            //In case the wifi disconnects and the semaphore is active it could lead to problems
    		  if(xSemaphoreTake(connectionSemaphore, 10000/portTICK_PERIOD_MS)){
    			printf("semaphore removed\n");
    		}
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


void gpio_init(){
	gpio_pad_select_gpio(FLASHLED);
	gpio_set_direction(FLASHLED, GPIO_MODE_OUTPUT);
	gpio_set_level(FLASHLED, 0);
	gpio_pad_select_gpio(ECHO);
	gpio_set_direction(ECHO, GPIO_MODE_INPUT);
	gpio_pad_select_gpio(TRIGGER);
	gpio_set_direction(TRIGGER, GPIO_MODE_OUTPUT);
	gpio_set_level(TRIGGER, 0);
}


void app_main(void)
{  //inizialize flash memory
    nvs_flash_init();


    ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );


   //initialize camera
    camera_start();


    tcpip_adapter_init();
    //create binary that is activated when connection is enstablished
    connectionSemaphore=xSemaphoreCreateBinary();
	 wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
	 ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_config));
	 ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE));


    //at the starting it generates its own wifi for configuration
	 connectAP();
	 RegisterEndPoints();
     gpio_init();


    speedSemaphore=xSemaphoreCreateBinary();
    //set a starting speed limit
    speedLimit=3;

    //core 0 will measure the speed, core 1 will send the data to tangle
	xTaskCreatePinnedToCore(UltrasonicTask,"Ultrasonic task",1024*10,NULL,1,NULL,0);
	xTaskCreatePinnedToCore(ShootTask,"Shoot task",1024*100,NULL,6,NULL,1);

}

