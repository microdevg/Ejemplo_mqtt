#include <stdio.h>
#include "com.h"



#include "esp_event.h"
#include <nvs_flash.h>
#include "protocol_examples_common.h"
#include "esp_netif.h"
#include "mqtt_client.h"



#define BROKER_URL "mqtt://192.168.0.103"
#define MQTT_ID   "admin"
#define MQTT_PASS "protocoloiot"
static esp_mqtt_client_handle_t client = NULL;



static void log_error_if_nonzero(const char *message, int error_code)
{
    if (error_code != 0) {
        printf( "Last error %s: 0x%x", message, error_code);
    }
}


/*
 * @brief Event handler registered to receive MQTT events
 *
 *  This function is called by the MQTT client event loop.
 *
 * @param handler_args user data registered to the event.
 * @param base Event base for the handler(always MQTT Base in this example).
 * @param event_id The id for the received event.
 * @param event_data The data for the event, esp_mqtt_event_handle_t.
 */
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    printf( "Event dispatched from event loop base=%s, event_id=%" PRIi32 "", base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;


    switch(event_id) {
        
    case MQTT_EVENT_CONNECTED:

        printf("nos conectamos a MQTT\n");
        esp_mqtt_client_subscribe(client, "casa/planta", 2);

    break;
    case MQTT_EVENT_DISCONNECTED:
       printf("Nos desconectamos de MQTT\n");

        break;

    case MQTT_EVENT_SUBSCRIBED:
        printf("\nSubscripcion correcta\n");
        
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        printf("MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        printf("MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        int len_d = event->data_len;
        int len_t = event->topic_len;
        static char b_data[100]={0};
        static char b_topic[100]={0};
        
        strncpy(b_data,event->data,len_d); 
        strncpy(b_topic,event->topic,len_t);

        b_data[len_d]= 0;
        b_topic[len_t]= 0;

        printf("LLego mensaje del topico %s: %s\n",b_topic,b_data);
     
    
        break;

    case MQTT_EVENT_ERROR:
        printf("Error, liberamos recursos del esp_mqtt\n");
        if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
            log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
            log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
            log_error_if_nonzero("captured as transport's socket errno",  event->error_handle->esp_transport_sock_errno);
            printf("Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));
        }
        break;
    default:
        printf("Evento no soportado:%d", event->event_id);
        break;
    }
}


//    esp_mqtt_client_publish(_client, topic, msg, 0, qos, retain);

void com_init(void)
{

// Conectarme WiFi, 
  ESP_ERROR_CHECK( nvs_flash_init() );
  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  ESP_ERROR_CHECK(example_connect());

  


//tengo que estar conectado y luego llama a mqtt init

esp_mqtt_client_config_t mqtt_cfg = {
    .broker.address.uri = BROKER_URL,
    .broker.address.port = 1883, // Cambia por tu puerto (ej. 1883 para MQTT sin cifrar, 8883 para MQTTS)
    .credentials.username = MQTT_ID,
    .credentials.authentication.password = MQTT_PASS,
};
    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);


}


void com_deinit(void){

}