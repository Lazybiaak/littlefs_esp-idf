#include "littlefs_esp_idf.h"
#include <stdio.h>
#include <string.h>
#include "esp_littlefs.h"
#include "esp_err.h"

#define MOUNT_POINT "/littlefs"

static bool is_mounted = false;

// Initialize LittleFS
esp_err_t littlefs_init(void)
{
    if (is_mounted) return ESP_OK;

    esp_vfs_littlefs_conf_t conf = {
        .base_path = MOUNT_POINT,
        .partition_label = "storage",
        .format_if_mount_failed = true,
        .dont_mount = false,
    };

    esp_err_t ret = esp_vfs_littlefs_register(&conf);

    if (ret == ESP_OK) {
        is_mounted = true;
    }

    return ret;
}

// Write file
esp_err_t littlefs_write_file(const char *name, const char *data, size_t len)
{
    char path[128];
    snprintf(path, sizeof(path), "%s/%s", MOUNT_POINT, name);

    FILE *f = fopen(path, "w");
    if (!f) return ESP_FAIL;

    fwrite(data, 1, len, f);
    fclose(f);

    return ESP_OK;
}

// Read file
esp_err_t littlefs_read_file(const char *name, char *buf, size_t max, size_t *read_len)
{
    char path[128];
    snprintf(path, sizeof(path), "%s/%s", MOUNT_POINT, name);

    FILE *f = fopen(path, "r");
    if (!f) return ESP_FAIL;

    *read_len = fread(buf, 1, max - 1, f);
    buf[*read_len] = '\0';

    fclose(f);
    return ESP_OK;
}

// Delete file
esp_err_t littlefs_delete_file(const char *name)
{
    char path[128];
    snprintf(path, sizeof(path), "%s/%s", MOUNT_POINT, name);

    return (remove(path) == 0) ? ESP_OK : ESP_FAIL;
}

// Check existence
bool littlefs_file_exists(const char *name)
{
    char path[128];
    snprintf(path, sizeof(path), "%s/%s", MOUNT_POINT, name);

    FILE *f = fopen(path, "r");
    if (f) {
        fclose(f);
        return true;
    }
    return false;
}

// Get storage info
esp_err_t littlefs_get_info(size_t *total, size_t *used)
{
    return esp_littlefs_info("storage", total, used);
}

// Deinitialize
esp_err_t littlefs_deinit(void)
{
    if (!is_mounted) return ESP_OK;

    esp_err_t ret = esp_vfs_littlefs_unregister("storage");

    if (ret == ESP_OK) {
        is_mounted = false;
    }

    return ret;
}