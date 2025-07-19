#ifndef __HOMEKIT_DEBUG_H__
#define __HOMEKIT_DEBUG_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdlib.h>
#include <stdio.h>
#include "Arduino.h"
#include <string.h>
#include <esp_xpgm.h>

    typedef unsigned char byte;

#define HOMEKIT_NO_LOG 0
#define HOMEKIT_LOG_ERROR 1
#define HOMEKIT_LOG_INFO 2
#define HOMEKIT_LOG_DEBUG 3

#ifdef LOG_MSG_BUFFER
// Added for RATGDO project that has custom logger function.
// Add -D LOG_MSG_BUFFER to compiler / platformio.ini
#ifndef HOMEKIT_LOG_LEVEL
#define HOMEKIT_LOG_LEVEL HOMEKIT_LOG_DEBUG
#endif
#ifndef ESP_LOG_LEVEL_T
#define ESP_LOG_LEVEL_T
    typedef enum
    {
        ESP_LOG_NONE,   /*!< No log output */
        ESP_LOG_ERROR,  /*!< Critical errors, software module can not recover on its own */
        ESP_LOG_WARN,   /*!< Error conditions from which recovery measures have been taken */
        ESP_LOG_INFO,   /*!< Information messages which describe normal flow of events */
        ESP_LOG_DEBUG,  /*!< Extra information which is not necessary for normal use (values, pointers, sizes, etc). */
        ESP_LOG_VERBOSE /*!< Bigger chunks of debugging information, or frequent messages which can potentially flood the output. */
    } esp_log_level_t;
#endif

    extern esp_log_level_t logLevel;
    void logToBuffer_P(const char *fmt, ...);
#define RATGDO_PRINTF(level, message, ...)               \
    do                                                   \
    {                                                    \
        if (level <= logLevel)                           \
            logToBuffer_P(PSTR(message), ##__VA_ARGS__); \
    } while (0)
#define ESP_LOGE(tag, message, ...) RATGDO_PRINTF(ESP_LOG_ERROR, "E (%lu) %s: " message "\r\n", millis(), tag, ##__VA_ARGS__)
#define ESP_LOGW(tag, message, ...) RATGDO_PRINTF(ESP_LOG_WARN, "W (%lu) %s: " message "\r\n", millis(), tag, ##__VA_ARGS__)
#define ESP_LOGI(tag, message, ...) RATGDO_PRINTF(ESP_LOG_INFO, "I (%lu) %s: " message "\r\n", millis(), tag, ##__VA_ARGS__)
#define ESP_LOGD(tag, message, ...) RATGDO_PRINTF(ESP_LOG_DEBUG, "D (%lu) %s: " message "\r\n", millis(), tag, ##__VA_ARGS__)
#define ESP_LOGV(tag, message, ...) RATGDO_PRINTF(ESP_LOG_VERBOSE, "V (%lu) %s: " message "\r\n", millis(), tag, ##__VA_ARGS__)

#define VERBOSE(message, ...) ESP_LOGV("HomeKit", "(%s) " message, __func__, ##__VA_ARGS__)
#define DEBUG(message, ...) ESP_LOGD("HomeKit", "(%s) " message, __func__, ##__VA_ARGS__)
    static uint32_t start_time = 0;
#define DEBUG_TIME_BEGIN() start_time = millis();
#define DEBUG_TIME_END(func_name) DEBUG("%s took %6lu ms\n", func_name, (millis() - start_time));
#define DEBUG_HEAP() DEBUG("Free heap: %d", system_get_free_heap_size());
#define ERROR(message, ...) ESP_LOGE("HomeKit", message, ##__VA_ARGS__)
#define INFO(message, ...) ESP_LOGI("HomeKit", message, ##__VA_ARGS__)
#define INFO_HEAP() INFO("Free heap: %d", system_get_free_heap_size());

#else // LOG_MSG_BUFFER

#ifndef HOMEKIT_LOG_LEVEL
#define HOMEKIT_LOG_LEVEL HOMEKIT_LOG_INFO
#endif
#define HOMEKIT_PRINTF XPGM_PRINTF

#if HOMEKIT_LOG_LEVEL >= HOMEKIT_LOG_DEBUG

#define VERBOSE(message, ...) HOMEKIT_PRINTF(">>> %s: " message "\n", __func__, ##__VA_ARGS__)
#define DEBUG(message, ...) HOMEKIT_PRINTF(">>> %s: " message "\n", __func__, ##__VA_ARGS__)
static uint32_t start_time = 0;
#define DEBUG_TIME_BEGIN() start_time = millis();
#define DEBUG_TIME_END(func_name) HOMEKIT_PRINTF("### [%7lu] %s took %6lu ms\n", millis(), func_name, (millis() - start_time));
#define DEBUG_HEAP() DEBUG("Free heap: %d", system_get_free_heap_size());

#else

#define DEBUG(message, ...)
#define DEBUG_TIME_BEGIN()
#define DEBUG_TIME_END(func_name)
#define DEBUG_HEAP()

#endif

#if HOMEKIT_LOG_LEVEL >= HOMEKIT_LOG_ERROR

#define ERROR(message, ...) HOMEKIT_PRINTF("!!! [%7lu] HomeKit: " message "\n", millis(), ##__VA_ARGS__)

#else

#define ERROR(message, ...)

#endif

#if HOMEKIT_LOG_LEVEL >= HOMEKIT_LOG_INFO

#define INFO(message, ...) HOMEKIT_PRINTF(">>> [%7lu] HomeKit: " message "\n", millis(), ##__VA_ARGS__)
#define INFO_HEAP() INFO("Free heap: %d", system_get_free_heap_size());

#else

#define INFO(message, ...)
#define INFO_HEAP()

#endif

#endif // LOG_MSG_BUFFER

    char *binary_to_string(const byte *data, size_t size);
    void print_binary(const char *prompt, const byte *data, size_t size);

#ifdef __cplusplus
}
#endif

#endif // __HOMEKIT_DEBUG_H__
