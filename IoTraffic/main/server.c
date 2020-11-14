/*
 * server.c
 *

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


#define TAG "server"


//address in memory of the web pages of the dashboard
extern const char indiceStart[] asm("_binary_index_html_start");
extern const char wifiStart[] asm("_binary_wifi_html_start");
extern const char gatewayStart[] asm("_binary_gateway_html_start");
extern const char channelStart[] asm("_binary_channel_html_start");

extern const char BootCssStart[] asm("_binary_bootstrap_min_css_start");
extern const char queryJsStart[] asm("_binary_jquery_min_js_start");
extern const char BootJsStart[] asm("_binary_bootstrap_min_js_start");

extern const char coverStart[] asm("_binary_cover_png_start");
extern const char coverFine[] asm("_binary_cover_png_end");



static esp_err_t on_url_hit(httpd_req_t *req)
{


	const unsigned int imgSize = coverFine - coverStart;

    printf("%s",req->uri);
    char path[600];
    sprintf(path, "%s", req->uri);

    if (strcmp(path, "/") == 0)
    {
        sprintf(path, "%s", "index.html");
    }
    ESP_LOGI(TAG, "url %s was hit", req->uri);

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


    if (strcmp(req->uri, "/channel.html") == 0)
    {

        httpd_resp_sendstr(req,channelStart);
        return ESP_OK;
    }



    if (strcmp(req->uri, "/gateway.html") == 0)
    {

        httpd_resp_sendstr(req,gatewayStart);
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
    ESP_LOGI(TAG, "url %s was hit", req->uri);
    char buf[50];
    memset(&buf, 0, sizeof(buf));
    httpd_req_recv(req, buf, req->content_len);
    cJSON *payload = cJSON_Parse(buf);

    char bufferserver[20];
    char bufferpass[20];

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
    ESP_LOGI(TAG, "url %s was hit", req->uri);
    char buf[50];
    memset(&buf, 0, sizeof(buf));
    httpd_req_recv(req, buf, req->content_len);
    printf("%s risposta JSON\n",buf);
    cJSON *payload = cJSON_Parse(buf);

    char buffergateway[105];


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

static esp_err_t on_channel_set(httpd_req_t *req)
{
    ESP_LOGI(TAG, "url %s was hit", req->uri);
    char buf[50];
    memset(&buf, 0, sizeof(buf));
    httpd_req_recv(req, buf, req->content_len);
    printf("%s risposta JSON\n",buf);
    cJSON *payload = cJSON_Parse(buf);

    char bufferchannel[105];


    cJSON *server = cJSON_GetObjectItem(payload, "channel");

    strcpy(bufferchannel, server->valuestring);

    nvs_handle_t nvs;
    nvs_open("ServerCreds", NVS_READWRITE, &nvs);
    nvs_set_str(nvs, "channel", bufferchannel);
    nvs_close(nvs);

    httpd_resp_set_status(req, "204 NO CONTENT");
    httpd_resp_send(req, NULL, 0);

    cJSON_Delete(payload);





    return ESP_OK;


}

extern int dutyTraffic;

static esp_err_t on_timing_set(httpd_req_t *req)
{
    ESP_LOGI(TAG, "url %s was hit", req->uri);
    char buf[50];
    memset(&buf, 0, sizeof(buf));
    httpd_req_recv(req, buf, req->content_len);
    printf("%s risposta JSON\n",buf);
    cJSON *payload = cJSON_Parse(buf);




    cJSON *value = cJSON_GetObjectItem(payload, "timing");


    dutyTraffic=value->valueint;
    httpd_resp_set_status(req, "204 NO CONTENT");
    httpd_resp_send(req, NULL, 0);

    cJSON_Delete(payload);





    return ESP_OK;


}



httpd_handle_t server = NULL;


extern char channelNVS[105];
extern char gatewayNVS[30];
void getGateway(){

	            nvs_handle_t nvs;
			    nvs_open("ServerCreds", NVS_READWRITE, &nvs);

			    size_t gateLen;
			    size_t channelLen;
			    nvs_get_str(nvs, "gateway", NULL, &gateLen);
			    nvs_get_str(nvs, "channel", NULL, &gateLen);

			    if(nvs_get_str(nvs, "gateway", NULL, &gateLen)==ESP_OK){
			    	if(gateLen>0){
			    		nvs_get_str(nvs, "gateway", gatewayNVS, &gateLen);
			    		printf("gateway ottenuto %s\n",gatewayNVS);
			    	}

			    }

			    if(nvs_get_str(nvs, "channel", NULL, &channelLen)==ESP_OK){
			    	if(channelLen>0){
			    		nvs_get_str(nvs, "channel", channelNVS, &channelLen);
			    		printf("channel ottenuto %s\n",channelNVS);
			    	}

			    }

}

void resetWifi(void *params)
{
vTaskDelay(1000 / portTICK_PERIOD_MS);
esp_wifi_stop();
getGateway();
vTaskDelay(1000 / portTICK_PERIOD_MS);
connectSTA();
httpd_stop(server);

vTaskDelete(NULL);
}


static esp_err_t on_start_request(httpd_req_t *req){

	printf("I am here\n");
	xTaskCreate(resetWifi, "reset wifi", 1024 * 2, NULL, 15, NULL);
    httpd_resp_set_status(req, "204 NO CONTENT");
    httpd_resp_send(req, NULL, 0);
	return ESP_OK;
}




void RegisterEndPoints(void)
{




    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.uri_match_fn = httpd_uri_match_wildcard;
    config.lru_purge_enable=true;
    config.stack_size=1024*20;//*10
    config.core_id=1;
    config.max_uri_handlers=20;






    ESP_LOGI(TAG, "starting server");
    if (httpd_start(&server, &config) != ESP_OK)
    {
    ESP_LOGE(TAG, "COULD NOT START SERVER");

    }









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

    httpd_uri_t channel_end_point_config = {
        .uri = "/api/channel",
        .method = HTTP_POST,
        .handler = on_channel_set};
    httpd_register_uri_handler(server, &channel_end_point_config);

    httpd_uri_t timing_end_point_config = {
        .uri = "/api/timing",
        .method = HTTP_POST,
        .handler = on_timing_set};
    httpd_register_uri_handler(server, &timing_end_point_config);


    httpd_uri_t first_end_point_config = {
        .uri = "/*",
        .method = HTTP_GET,
        .handler = on_url_hit};
    httpd_register_uri_handler(server, &first_end_point_config);




}
