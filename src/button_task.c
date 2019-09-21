#include "espressif/esp_common.h"
#include "esp/uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "esp8266.h"

#include <string.h>
#include <paho_mqtt_c/MQTTClient.h>

#include "doorbell.h"

const gpio_inttype_t int_type = GPIO_INTTYPE_EDGE_NEG;
//const gpio_inttype_t int_type = GPIO_INTTYPE_LEVEL_LOW;
//const gpio_inttype_t int_type = GPIO_INTTYPE_EDGE_POS;


void gpio_intr_handler(uint8_t gpio_num)
{
    uint32_t now = xTaskGetTickCountFromISR();
    xQueueSendToBackFromISR(ts_queue, &now, NULL);
}


void button_task(void *pvParameters)
{
    gpio_enable(PIN_BUTTON, GPIO_INPUT); //There is already a pull-up
    gpio_set_pullup(PIN_BUTTON, true, false);

    gpio_enable(PIN_RELAY, GPIO_OUTPUT);
    gpio_write(PIN_RELAY, !RELAY_ACTIVE_LVL);

    printf("Waiting for button press interrupt on gpio %d...\r\n", PIN_BUTTON);
    //QueueHandle_t *ts_queue = (QueueHandle_t *)pvParameters;
    
    mqtt_message_data_t message_data;

    gpio_set_interrupt(PIN_BUTTON, int_type, gpio_intr_handler);

    uint32_t last = 0;
    while(1) {
        uint32_t button_ts;
        xQueueReceive(ts_queue, &button_ts, portMAX_DELAY);
        button_ts *= portTICK_PERIOD_MS;
        if ((button_ts - last) > 1000) {
            printf("Button pressed at %d ms, last at %d ms\r\n", button_ts, last);
        
            /* Notify the relay task to switch on-off the relay */
            //xTaskNotifyGive( relayTaskHandle );
        
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

            xQueueReset(ts_queue);
            last = button_ts;
        }
    }
}



