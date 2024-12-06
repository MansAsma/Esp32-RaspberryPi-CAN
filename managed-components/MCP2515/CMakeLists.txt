
set(srcs "src/mcp2515.c")
set(public_requires "esp_driver_spi")
idf_component_register(SRCS ${srcs}
                       INCLUDE_DIRS "./Include"
                       REQUIRES ${public_requires})
