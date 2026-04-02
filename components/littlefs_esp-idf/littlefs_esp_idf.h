#ifndef LITTLEFS_ESP_IDF_H
#define LITTLEFS_ESP_IDF_H

#include <stddef.h>
#include <stdbool.h>
#include "esp_err.h"

esp_err_t littlefs_init(void);
esp_err_t littlefs_write_file(const char *name, const char *data, size_t len);
esp_err_t littlefs_read_file(const char *name, char *buf, size_t max, size_t *read_len);
esp_err_t littlefs_delete_file(const char *name);
bool littlefs_file_exists(const char *name);
esp_err_t littlefs_get_info(size_t *total, size_t *used);
esp_err_t littlefs_deinit(void);

#endif