set(srcs "src/dht.c")
set(requires esp_driver_gpio  )
idf_component_register(SRCS ${srcs}
                       INCLUDE_DIRS ./Include/ ${IDF_PATH}/components/esp_rom/include/esp32/rom/
                       REQUIRES ${requires})