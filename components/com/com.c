#include <stdio.h>
#include "com.h"



#include "esp_event.h"
#include <nvs_flash.h>
#include "protocol_examples_common.h"
#include "esp_netif.h"



void com_init(void)
{

// Conectarme WiFi, 
  ESP_ERROR_CHECK( nvs_flash_init() );
  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  ESP_ERROR_CHECK(example_connect());



//tengo que estar conectado y luego llama a mqtt init




}


void com_deinit(void){

}