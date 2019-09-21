#include "espressif/esp_common.h"
#include "esp/uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "esp8266.h"

#include <string.h>
#include <paho_mqtt_c/MQTTClient.h>

#include "doorbell.h"

void relay_task(void *pvParameters)
{
    mqtt_message_data_t message_data;

    gpio_enable(PIN_RELAY, GPIO_OUTPUT);
    gpio_write(PIN_RELAY, !RELAY_ACTIVE_LVL);

    while(1) {
        ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
        
        gpio_write(PIN_RELAY, RELAY_ACTIVE_LVL);
        put_mqtt_message_to_queue(&message_data, 
                MQTT_STATE_TOPIC, strlen(MQTT_STATE_TOPIC), 
                MQTT_STATE_MESSAGE_ON, strlen(MQTT_STATE_MESSAGE_ON), 
                MQTT_QOS1);
        
        vTaskDelay( RELAY_DELAY_ON_MS / portTICK_PERIOD_MS );
        
        gpio_write(PIN_RELAY, !RELAY_ACTIVE_LVL);
        put_mqtt_message_to_queue(&message_data, 
                MQTT_STATE_TOPIC, strlen(MQTT_STATE_TOPIC), 
                MQTT_STATE_MESSAGE_OFF, strlen(MQTT_STATE_MESSAGE_OFF), 
                MQTT_QOS1);
    }
}
