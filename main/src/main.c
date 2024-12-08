#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "mcp2515.h"
#include "dht.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_intr_alloc.h"
#include "esp_log.h"
#include "esp_task_wdt.h"

#define TAG "CAN_MODULE"

// SPI pin definitions
#define PIN_NUM_MISO 19
#define PIN_NUM_MOSI 23
#define PIN_NUM_CLK 18
#define PIN_NUM_CS 5
#define PIN_NUM_INTERRUPT 27

// DHT11 sensor GPIO pin
#define DHT_GPIO_PIN 26

// CAN frame IDs for temperature and humidity
#define TEMPERATURE_CAN_ID 0x36
#define HUMIDITY_CAN_ID 0x40

// Define the type of DHT sensor being used
#define SENSOR_TYPE DHT_TYPE_DHT11

// Function to initialize the SPI bus
static bool SPI_init(void)
{
	ESP_LOGI(TAG, "SPI init");
	esp_err_t ret;

	// Configuration for the SPI bus
	spi_bus_config_t bus_cfg = {
		.miso_io_num = PIN_NUM_MISO,
		.mosi_io_num = PIN_NUM_MOSI,
		.sclk_io_num = PIN_NUM_CLK,
		.quadwp_io_num = -1,
		.quadhd_io_num = -1,
		.max_transfer_sz = 4096};

	// Define MCP2515 SPI device configuration
	spi_device_interface_config_t dev_cfg = {
		.mode = 0,						   // (0,0)
		.clock_speed_hz = 1 * 1000 * 1000, // 1 mhz
		.spics_io_num = PIN_NUM_CS,
		.queue_size = 7,
		.flags = 0};

	// Initialize SPI bus
	ret = spi_bus_initialize(SPI3_HOST, &bus_cfg, SPI_DMA_CH_AUTO);
	ESP_ERROR_CHECK(ret);

	// Add MCP2515 SPI device to the bus
	ret = spi_bus_add_device(SPI3_HOST, &dev_cfg, &MCP2515_Object->spi);
	ESP_ERROR_CHECK(ret);

	return true;
}

void app_main(void)
{
	// Initialize the MCP2515
	MCP2515_init();
	// Initialize the SPI bus
	SPI_init();
	MCP2515_reset();
	// Set Can bitrate and clck speed
	MCP2515_setBitrate(CAN_500KBPS, MCP_8MHZ);
	// Set the MCP2515 to normal mode
	MCP2515_setNormalMode();

	// Variable to store temperature value
	int16_t temperatureVal = 0;
	// Variable to store humidity value
	int16_t humidityVal = 0;

	// CAN frame for temperature data
	CAN_FRAME_t temp_can_frame_rx[1], humidity_can_frame_rx[1];
	while (1)
	{
		// Read temperature data from DHT sensor
		esp_err_t retReadData = dht_read_data(SENSOR_TYPE, DHT_GPIO_PIN, &humidityVal, &temperatureVal);
		if (ESP_OK != retReadData)
		{
			ESP_LOGE(TAG, "Couldn't read dht data, retReadData value: %d", retReadData);
		}
		else
		{
			// Prepare CAN frame with temperature data
			temp_can_frame_rx[0]->can_id = TEMPERATURE_CAN_ID;
			temp_can_frame_rx[0]->can_dlc = 2;							  // Data length code
			temp_can_frame_rx[0]->data[0] = (temperatureVal >> 8) & 0xFF; // MSB
			temp_can_frame_rx[0]->data[1] = temperatureVal & 0xFF;		  // LSB

			// Prepare CAN frame with humidity data
			humidity_can_frame_rx[0]->can_id = HUMIDITY_CAN_ID;
			humidity_can_frame_rx[0]->can_dlc = 2;
			humidity_can_frame_rx[0]->data[0] = (humidityVal >> 8) & 0xFF; // MSB
			humidity_can_frame_rx[0]->data[1] = humidityVal & 0xFF;		   // LSB
		}

		// Send the temperature message over CAN
		ERROR_t ret_val = MCP2515_sendMessageAfterCtrlCheck(temp_can_frame_rx[0]);

		if (ret_val != ERROR_OK)
		{
			ESP_LOGE(TAG, "Couldn't send Temperature message ret value: %d", ret_val);
		}
		else
		{
			ESP_LOGI(TAG, "Temperature Message is sent data MSB: %x, LSB: %x", temp_can_frame_rx[0]->data[0], temp_can_frame_rx[0]->data[1]);
		}

		// Delay for 3 seconds
		vTaskDelay(pdMS_TO_TICKS(3000));

		// Send the temperature message over CAN
		ret_val = MCP2515_sendMessageAfterCtrlCheck(humidity_can_frame_rx[0]);

		if (ret_val != ERROR_OK)
		{
			ESP_LOGE(TAG, "Couldn't send humidity message ret value: %d", ret_val);
		}
		else
		{
			ESP_LOGI(TAG, "humidity Message is sent data MSB: %x, LSB: %x", humidity_can_frame_rx[0]->data[0], humidity_can_frame_rx[0]->data[1]);
		}

		// Delay for 60 seconds
		vTaskDelay(pdMS_TO_TICKS(60000));
	}
}
