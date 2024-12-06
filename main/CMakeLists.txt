set(srcs "src/main.c")
set(public_requires "esp_driver_spi")
set(private_requires "esp_driver_gpio")
idf_component_register(SRCS ${srcs}
                    INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/managed-components/MCP2515/Include" "${CMAKE_SOURCE_DIR}/managed-components/DHT11/Include"
                    REQUIRES ${public_requires}
                    PRIV_REQUIRES ${private_requires})