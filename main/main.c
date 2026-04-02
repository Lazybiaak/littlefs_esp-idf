#include <stdio.h>
#include <string.h>
#include "littlefs_esp_idf.h"
#include "driver/uart.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define UART_PORT UART_NUM_0
#define BUF_SIZE 1024

void app_main(void)
{
    // Init LittleFS
    littlefs_init();

    // UART config (default UART0 already connected to USB)
    uart_driver_install(UART_PORT, BUF_SIZE, 0, 0, NULL, 0);

    uint8_t data[100];

    while (1)
    {
        int len = uart_read_bytes(UART_PORT, data, sizeof(data) - 1, 1000 / portTICK_PERIOD_MS);

        if (len > 0)
        {
            data[len] = '\0'; // null terminate

            // 👉 CONDITION: if first char is '0' → READ
            if (data[0] == '0')
            {
                char buf[100] = {0};
                size_t read = 0;

                if (littlefs_read_file("test.txt", buf, sizeof(buf), &read) == ESP_OK)
                {
                    printf("Read: %s\n", buf);
                }
                else
                {
                    printf("Read failed\n");
                }
            }
            else
            {
                // 👉 WRITE
                littlefs_write_file("test.txt", (char *)data, len);
                printf("Written: %s\n", data);
            }
        }

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}