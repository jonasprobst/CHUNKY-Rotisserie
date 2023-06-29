#include "NVSSettings.h"
#include <nvs_flash.h>
#include <nvs.h>
#include <esp_log.h>

NVSSettings::NVSSettings() : xSemaphore_(xSemaphoreCreateMutex()) {
    initialize();
}

NVSSettings::~NVSSettings() {
    vSemaphoreDelete(xSemaphore_);
}

void NVSSettings::initialize() {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_LOGE("NVSSettings:", "An error occurred during NVS initialization. Erasing flash and trying again.");
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

uint16_t NVSSettings::getBaseChannel() const {
    uint16_t base_channel = 0;
    nvs_handle_t my_handle;
    xSemaphoreTake(xSemaphore_, portMAX_DELAY);
    if (nvs_open("storage", NVS_READONLY, &my_handle) == ESP_OK) {
        int32_t base_channel32;
        if (nvs_get_i32(my_handle, "baseChannel", &base_channel32) == ESP_OK) {
            base_channel = static_cast<uint16_t>(base_channel32);
            ESP_LOGD("NVSSettings:", "read NVS succesful. BC: %d", base_channel);
        }
        nvs_close(my_handle);
    }
    xSemaphoreGive(xSemaphore_);
    return base_channel;
}

uint16_t NVSSettings::getMode() const {
    uint16_t mode = 0;
    nvs_handle_t my_handle;
    xSemaphoreTake(xSemaphore_, portMAX_DELAY);
    if (nvs_open("storage", NVS_READONLY, &my_handle) == ESP_OK) {
        int32_t mode32;
        if (nvs_get_i32(my_handle, "mode", &mode32) == ESP_OK) {
            mode = static_cast<uint16_t>(mode32);
            ESP_LOGD("NVSSettings:", "read NVS sucessfull. M: %d", mode);
        }
        nvs_close(my_handle);
    }
    xSemaphoreGive(xSemaphore_);
    return mode;
}

void NVSSettings::save(uint16_t base_channel, uint16_t mode) {
    nvs_handle_t my_handle;
    xSemaphoreTake(xSemaphore_, portMAX_DELAY);
    if (nvs_open("storage", NVS_READWRITE, &my_handle) == ESP_OK) {
        nvs_set_i32(my_handle, "baseChannel", base_channel);
        nvs_set_i32(my_handle, "mode", mode);
        nvs_commit(my_handle);
        nvs_close(my_handle);
        ESP_LOGD("NVSSettings:", "saved M: %d, BC: %d", base_channel, mode);
    }
    xSemaphoreGive(xSemaphore_);
}
