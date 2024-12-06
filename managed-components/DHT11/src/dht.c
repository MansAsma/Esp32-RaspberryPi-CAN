
/**
 * @file dht.c
 *
 * ESP-IDF driver for DHT11, AM2301 (DHT21, DHT22, AM2302, AM2321), Itead Si7021
 *
 * Ported from esp-open-rtos
 *
 * Copyright (c) 2016 Jonathan Hartsuiker <https://github.com/jsuiker>\n
 * Copyright (c) 2018 Ruslan V. Uss <unclerus@gmail.com>\n
 *
 * BSD Licensed as described in the file LICENSE
 */
#include "dht.h"

#include <freertos/FreeRTOS.h>
#include <string.h>
#include <esp_log.h>
#include <ets_sys.h>
#include <esp_idf_lib_helpers.h>
