#ifndef MYTASKS_H
#define MYTASKS_H

#include <task.h>
#include <semphr.h>
#include <arpa/inet.h>
#include <paho_mqtt_c/MQTTClient.h>

#define FIRMWARE_VERSION "0.0.1"

extern SemaphoreHandle_t wifi_alive;
extern QueueHandle_t publish_queue;
extern QueueHandle_t ts_queue;
extern TaskHandle_t switchTaskHandle;
extern TaskHandle_t mqttTaskHandle;
extern TaskHandle_t buttonTaskHandle;
extern TaskHandle_t relayTaskHandle;

extern void wifi_task(void *pvParameters);
extern void beat_task(void *pvParameters);
extern void mqtt_task(void *pvParameters);
extern void button_task(void *pvParameters);
extern void relay_task(void *pvParameters);

extern bool put_mqtt_message_to_queue(mqtt_message_data_t *mqtt_message, const char* _topic, uint16_t topic_length, const char* _payload, uint16_t payload_length, char qos);

#define MQTT_HOST "192.168.1.2"
#define MQTT_PORT 1883
#define MQTT_SUBSCRIBE_TOPIC "doorbell/set"
#define MQTT_STATE_TOPIC "doorbell"
#define MQTT_STATE_MESSAGE_ON "on"
#define MQTT_STATE_MESSAGE_OFF "off"
#define MQTT_STATUS_TOPIC "doorbell/status"
#define MQTT_ATTRIBUTES_TOPIC "doorbell/attributes"
#define MQTT_STATUS_ONLINE "online"
#define MQTT_STATUS_OFFLINE "offline"

#define MQTT_ATTRIBUTES_PUBLISH_PERIOD 10*60*1000 //ms

#define MQTT_MESSAGE_LEN 128  // Max length of an mqtt message

#define PIN_RELAY 0 //D3
#define PIN_BUTTON 2 //D4

#define RELAY_DELAY_ON_MS 300
#define RELAY_ACTIVE_LVL false

#endif //MYTASKS_H

