#include <string.h>

#include <paho_mqtt_c/MQTTESP8266.h>
#include <paho_mqtt_c/MQTTClient.h>

#include "doorbell.h"


bool put_mqtt_message_to_queue(mqtt_message_data_t *mqtt_message, const char* _topic, uint16_t topic_length, const char* _payload, uint16_t payload_length, char qos)
{
    if (mqtt_message == NULL) {
        printf("mqtt_message_data_t must be initialized before using.");
        return false;
    }

    char* payload = (char*)malloc(payload_length + 1);
    if (payload == NULL) {
        printf("Not enough memory to create a payload string in an mqtt message");
        return false;
    }
    memset(payload, 0, (payload_length + 1));

    char* topic = (char*)malloc(topic_length + 1);
    if (topic == NULL) {
        printf("Not enough memory to create a topic string in an mqtt message");
        free(payload);
        return false;
    }
    memset(topic, 0, (topic_length + 1));

    strncpy( topic, _topic, topic_length );
    strncpy( payload, _payload, payload_length );

    mqtt_string_t *message_topic = (mqtt_string_t*)malloc( sizeof(mqtt_string_t) );
    if ( message_topic == NULL ) {
        free(payload);
        free(topic);
        printf("Not enough memory to create a topic in an mqtt message\n");
        return false;
    }

    mqtt_message_t *message = (mqtt_message_t*)malloc( sizeof(mqtt_message_t) );
    if ( message != NULL ) {
        message->payload = payload;
        message->payloadlen = payload_length;
        message->dup = 0;
        message->qos = qos;
        message->retained = 0;

        message_topic->lenstring.data = topic;
        message_topic->lenstring.len = topic_length;

        mqtt_message->topic = message_topic;
        mqtt_message->message = message;

        if (xQueueSend(publish_queue, mqtt_message, 0) == pdFALSE) {
            printf("Publish queue overflow.\n");
            return false;
        }
    } else {
        free(payload);
        free(topic);
        free(message_topic);
        printf("Not enough memory to create an mqtt message\n");
        return false;
    }

    return true;
}

