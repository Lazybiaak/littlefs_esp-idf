#include "littlefs_esp_idf.h"

void app_main(void)
{
    // Initialize
    littlefs_init();
    
    // Write
    littlefs_write_file("test.txt", "Hello", 5);
    
    // Read
    char buf[100] = {0};
size_t read = 0;

if (littlefs_read_file("test.txt", buf, sizeof(buf), &read) == ESP_OK) {
    printf("Read: %s\n", buf);
} else {
    printf("Read failed\n");
}
    
    // Cleanup
    littlefs_deinit();
}