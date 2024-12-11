#include <stdio.h>
#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <unistd.h>      // for usleep
#include "driver/i2c.h"  // for I2C communication
#include "esp_log.h"     // for ESP_LOGI
#include "lcd.h"
#include "dht.h"

#include "lcd.h"  // Thêm file header LCD của bạn

#define SENSOR_TYPE DHT_TYPE_DHT11
#define DHT_GPIO_PIN GPIO_NUM_4

void dht_test(void *pvParameters)
{
    float temperature, humidity;
    char buffer[16]; // Buffer cho chuỗi hiển thị
    ESP_ERROR_CHECK(i2c_master_init());
    ESP_LOGI(TAG, "I2C initialized successfully");
    lcd_init();      // Khởi tạo LCD
    lcd_clear();     // Xóa màn hình LCD ban đầu

    while (1)
    {
        if (dht_read_float_data(SENSOR_TYPE, DHT_GPIO_PIN, &humidity, &temperature) == ESP_OK)
        {
            // Hiển thị độ ẩm
            lcd_put_cur(0, 0); // Đặt con trỏ ở dòng 0, cột 0
            sprintf(buffer, "Humidity: %.2f%%", humidity);
            lcd_send_string(buffer);

            // Hiển thị nhiệt độ
            lcd_put_cur(1, 0); // Đặt con trỏ ở dòng 1, cột 0
            sprintf(buffer, "Temp: %.2fC", temperature);
            lcd_send_string(buffer);

            printf("Humidity: %.2f%% Temp: %.2fC\n", humidity, temperature);
        }
        else
        {
            printf("Could not read data from sensor\n");

            // Hiển thị lỗi lên LCD
            lcd_put_cur(0, 0);
            lcd_send_string("Sensor Error    "); // Hiển thị lỗi
        }

        vTaskDelay(pdMS_TO_TICKS(2000)); // Đợi 2 giây trước lần đọc tiếp theo
    }
}

void app_main(void)
{
    xTaskCreate(dht_test, "dht_test", configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL);
}

