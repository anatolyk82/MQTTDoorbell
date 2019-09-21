#include "espressif/esp_common.h"
#include "esp/uart.h"

#include <FreeRTOS.h>
#include <task.h>

#include <espressif/esp_sta.h>
#include <espressif/esp_wifi.h>

#include <ssid_config.h>

#include <semphr.h>

#include "doorbell.h"


void wifi_task(void *pvParameters)
{
    uint8_t status = 0;
    struct sdk_station_config config = {
        .ssid = WIFI_SSID,
        .password = WIFI_PASS,
    };

    printf("WiFi: connecting to WiFi\n\r");
    sdk_wifi_set_opmode(STATION_MODE);
    sdk_wifi_station_set_config(&config);

    while(1)
    {
        while (status != STATION_GOT_IP) {
            status = sdk_wifi_station_get_connect_status();
            printf("%s: status = %d\n\r", __func__, status );
            if( status == STATION_WRONG_PASSWORD ){
                printf("WiFi: wrong password\n\r");
                break;
            } else if( status == STATION_NO_AP_FOUND ) {
                printf("WiFi: AP not found\n\r");
                break;
            } else if( status == STATION_CONNECT_FAIL ) {
                printf("WiFi: connection failed\r\n");
                break;
            }
            vTaskDelay( 2000 / portTICK_PERIOD_MS );
        }
        if (status == STATION_GOT_IP) {
            printf("WiFi: Connected. Status = %d\n\r", status);
            xSemaphoreGive( wifi_alive );
            taskYIELD();
        }

        while ((status = sdk_wifi_station_get_connect_status()) == STATION_GOT_IP) {
            xSemaphoreGive( wifi_alive );
            taskYIELD();
        }
        printf("WiFi: Disconnected\n\r");
        vTaskDelay( 2000 / portTICK_PERIOD_MS );
    }
    sdk_wifi_station_disconnect();
}
