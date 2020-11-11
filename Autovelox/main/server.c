/*
 * server.c
 *
 *  Created on: 30 oct 2020
 *      Author: Raul Rosa
 */

#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "connect.h"
#include "esp_log.h"
#include "esp_http_server.h"
#include "server.h"
#include "cJSON.h"
#include "esp_camera.h"


#define TAG "server"


extern const char indiceStart[] asm("_binary_index_html_start");
extern const char wifiStart[] asm("_binary_wifi_html_start");
extern const char gatewayStart[] asm("_binary_gateway_html_start");
extern const char IDStart[] asm("_binary_deviceid_html_start");
extern const char limitStart[] asm("_binary_limit_html_start");

extern const char BootCssStart[] asm("_binary_bootstrap_min_css_start");
extern const char queryJsStart[] asm("_binary_jquery_min_js_start");
extern const char BootJsStart[] asm("_binary_bootstrap_min_js_start");

extern const char coverStart[] asm("_binary_cover_png_start");
extern const char coverFine[] asm("_binary_cover_png_end");



static esp_err_t on_url_hit(httpd_req_t *req)
{


	const unsigned int imgSize = coverFine - coverStart;
    char path[600];
    sprintf(path, "%s", req->uri);

    if (strcmp(path, "/") == 0)
    {
        sprintf(path, "%s", "index.html");
    }

    //style.css

    char *ptr = strrchr(path, '.');
    if (strcmp(ptr, ".css") == 0)
    {
        httpd_resp_set_type(req, "text/css");
    }

    if (strcmp(ptr, ".map") == 0)
    {
        httpd_resp_send_404(req);
        return ESP_OK;
    }

    if (strcmp(req->uri, "/") == 0)
    {

        httpd_resp_sendstr(req,indiceStart);
        return ESP_OK;
    }

    if (strcmp(req->uri, "/index.html") == 0)
    {

        httpd_resp_sendstr(req,indiceStart);
        return ESP_OK;
    }


    if (strstr(req->uri, "/snapshot.jpg") !=NULL)
    {

        camera_fb_t * fb = esp_camera_fb_get();
        if (!fb) {

            return ESP_FAIL;
        }
    	httpd_resp_send(req,(char*)fb->buf,fb->len);
    	esp_camera_fb_return(fb);
        return ESP_OK;
    }


    if (strcmp(req->uri, "/gateway.html") == 0)
    {

        httpd_resp_sendstr(req,gatewayStart);
        return ESP_OK;
    }


    if (strcmp(req->uri, "/limit.html") == 0)
    {

        httpd_resp_sendstr(req,limitStart);
        return ESP_OK;
    }


    if (strcmp(req->uri, "/deviceid.html") == 0)
    {

        httpd_resp_sendstr(req,IDStart);
        return ESP_OK;
    }


    if (strcmp(req->uri, "/wifi.html") == 0)
    {

        httpd_resp_sendstr(req,wifiStart);
        return ESP_OK;
    }




    if (strcmp(req->uri, "/boot/css/bootstrap.min.css") == 0)
    {

        httpd_resp_sendstr(req,BootCssStart);
        return ESP_OK;
    }



    if (strcmp(req->uri, "/boot/js/bootstrap.min.js") == 0)
    {

        httpd_resp_sendstr(req,BootJsStart);
        return ESP_OK;
    }
    if (strcmp(req->uri, "/boot/jquery/jquery.min.js") == 0)
    {

        httpd_resp_sendstr(req,queryJsStart);
        return ESP_OK;
    }

    if (strcmp(req->uri, "/img/cover.png") == 0)
    {


    	httpd_resp_send(req,coverStart,imgSize);
        return ESP_OK;
    }


        httpd_resp_send_404(req);


    return ESP_OK;
}


static esp_err_t on_server_set(httpd_req_t *req)
{

    char buf[50];
    memset(&buf, 0, sizeof(buf));
    httpd_req_recv(req, buf, req->content_len);
    cJSON *payload = cJSON_Parse(buf);

    char bufferserver[30];
    char bufferpass[30];

    cJSON *server = cJSON_GetObjectItem(payload, "wifiserver");
    cJSON *pass = cJSON_GetObjectItem(payload, "passserver");




    strcpy(bufferserver, server->valuestring);


    strcpy(bufferpass, pass->valuestring);






    nvs_handle_t nvs;
    nvs_open("ServerCreds", NVS_READWRITE, &nvs);
    nvs_set_str(nvs, "password", bufferpass);
    nvs_set_str(nvs, "server", bufferserver);
    nvs_close(nvs);
    httpd_resp_set_status(req, "204 NO CONTENT");
    httpd_resp_send(req, NULL, 0);



    cJSON_Delete(payload);



    return ESP_OK;


}


static esp_err_t on_gateway_set(httpd_req_t *req)
{

    char buf[50];
    memset(&buf, 0, sizeof(buf));
    httpd_req_recv(req, buf, req->content_len);

    cJSON *payload = cJSON_Parse(buf);

    char buffergateway[30];


    cJSON *server = cJSON_GetObjectItem(payload, "gateway");


    strcpy(buffergateway, server->valuestring);

    nvs_handle_t nvs;
    nvs_open("ServerCreds", NVS_READWRITE, &nvs);
    nvs_set_str(nvs, "gateway", buffergateway);
    nvs_close(nvs);

    httpd_resp_set_status(req, "204 NO CONTENT");
    httpd_resp_send(req, NULL, 0);



    cJSON_Delete(payload);





    return ESP_OK;


}

static esp_err_t on_deviceid_set(httpd_req_t *req)
{

    char buf[50];
    memset(&buf, 0, sizeof(buf));
    httpd_req_recv(req, buf, req->content_len);
    cJSON *payload = cJSON_Parse(buf);

    char bufferid[50];


    cJSON *server = cJSON_GetObjectItem(payload, "deviceid");




    strcpy(bufferid, server->valuestring);




    nvs_handle_t nvs;
    nvs_open("ServerCreds", NVS_READWRITE, &nvs);
    nvs_set_str(nvs, "deviceid", bufferid);
    nvs_close(nvs);


    httpd_resp_set_status(req, "204 NO CONTENT");
    httpd_resp_send(req, NULL, 0);



    cJSON_Delete(payload);




    return ESP_OK;


}

static esp_err_t on_limit_set(httpd_req_t *req)
{

    char buf[50];
    memset(&buf, 0, sizeof(buf));
    httpd_req_recv(req, buf, req->content_len);
    cJSON *payload = cJSON_Parse(buf);

    char bufferlimit[50];


    cJSON *server = cJSON_GetObjectItem(payload, "limit");




    strcpy(bufferlimit, server->valuestring);



    nvs_handle_t nvs;
    nvs_open("ServerCreds", NVS_READWRITE, &nvs);
    nvs_set_str(nvs, "limit", bufferlimit);
    nvs_close(nvs);


    httpd_resp_set_status(req, "204 NO CONTENT");
    httpd_resp_send(req, NULL, 0);



    cJSON_Delete(payload);




    return ESP_OK;


}





httpd_handle_t server = NULL;

extern char gatewayNVS[20];
extern char deviceidNVS[20];

extern float speedLimit;
extern float speed;
void getNVS(){

	            nvs_handle_t nvs;
			    nvs_open("ServerCreds", NVS_READWRITE, &nvs);

			    size_t gateLen;
			    size_t idLen;
			    size_t limitLen;
			    nvs_get_str(nvs, "gateway", NULL, &gateLen);
			    nvs_get_str(nvs, "deviceid", NULL, &idLen);
			    nvs_get_str(nvs, "deviceid", NULL, &idLen);

			    if(nvs_get_str(nvs, "gateway", NULL, &gateLen)==ESP_OK){
			    	if(gateLen>0){
			    		nvs_get_str(nvs, "gateway", gatewayNVS, &gateLen);
			    		printf("gateway obtained %s\n",gatewayNVS);
			    	}

			    }

			    if(nvs_get_str(nvs, "deviceid", NULL, &idLen)==ESP_OK){
			    	if(idLen>0){
			    		nvs_get_str(nvs, "deviceid", deviceidNVS, &idLen);
			    		printf("id obtained  %s\n",deviceidNVS);
			    	}

			    }

			    if(nvs_get_str(nvs, "limit", NULL, &limitLen)==ESP_OK){
			    	if(idLen>0){
			    		char limitchar[5];
			    		nvs_get_str(nvs, "limit", limitchar, &limitLen);
			    		speedLimit=(float)atoi(limitchar);
			    		printf("limit obtained %s\n",limitchar);
			    	}

			    }

}

void resetWifi(void *params)
{
vTaskDelay(1000 / portTICK_PERIOD_MS);
esp_wifi_stop();
getNVS();
vTaskDelay(1000 / portTICK_PERIOD_MS);
connectSTA();
httpd_stop(server);

vTaskDelete(NULL);
}


static esp_err_t on_start_request(httpd_req_t *req){


	xTaskCreate(resetWifi, "reset wifi", 1024 * 2, NULL, 15, NULL);
    httpd_resp_set_status(req, "204 NO CONTENT");
    httpd_resp_send(req, NULL, 0);
	return ESP_OK;
}



static esp_err_t on_speed_get(httpd_req_t *req){
char message[50];
sprintf(message, "{\"speed\":%.2f}", speed);
httpd_resp_send(req, message, strlen(message));
return ESP_OK;
}

void RegisterEndPoints(void)
{




    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.uri_match_fn = httpd_uri_match_wildcard;
    config.lru_purge_enable=true;
    config.stack_size=1024*20;//*10
    config.core_id=1;//core used for endpoints
    config.max_uri_handlers=20;






    ESP_LOGI(TAG, "starting server");
    if (httpd_start(&server, &config) != ESP_OK)
    {
    ESP_LOGE(TAG, "COULD NOT START SERVER");

    }



    httpd_uri_t speed_end_point_config = {
        .uri = "/api/speed",
        .method = HTTP_GET,
        .handler = on_speed_get};
    httpd_register_uri_handler(server, &speed_end_point_config);





    httpd_uri_t server_end_point_config = {
        .uri = "/api/server",
        .method = HTTP_POST,
        .handler = on_server_set};
    httpd_register_uri_handler(server, &server_end_point_config);

    httpd_uri_t start_end_point_config = {
        .uri = "/api/start",
        .method = HTTP_GET,
        .handler = on_start_request};
    httpd_register_uri_handler(server, &start_end_point_config);

    httpd_uri_t gateway_end_point_config = {
        .uri = "/api/gateway",
        .method = HTTP_POST,
        .handler = on_gateway_set};
    httpd_register_uri_handler(server, &gateway_end_point_config);


    httpd_uri_t deviceid_end_point_config = {
        .uri = "/api/deviceid",
        .method = HTTP_POST,
        .handler = on_deviceid_set};
    httpd_register_uri_handler(server, &deviceid_end_point_config);


    httpd_uri_t limit_end_point_config = {
        .uri = "/api/limit",
        .method = HTTP_POST,
        .handler = on_limit_set};
    httpd_register_uri_handler(server, &limit_end_point_config);


    httpd_uri_t first_end_point_config = {
        .uri = "/*",
        .method = HTTP_GET,
        .handler = on_url_hit};
    httpd_register_uri_handler(server, &first_end_point_config);




}
