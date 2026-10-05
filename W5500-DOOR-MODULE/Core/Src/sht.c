/*
 * sht.c
 *
 *  Created on: Jul 24, 2026
 *      Author: HAIKAL
 */

#include "sht.h"

extern I2C_HandleTypeDef hi2c1;

void SHT31_ReadTempHumidity(float *temperature, float *humidity)
{
	uint8_t  data[6];
	uint8_t  cmd[2];
	uint16_t temp_raw, humidity_raw;

	// Send command to measure temperature
	cmd[0] = (uint8_t)(CMD_MEASURE_TEMP >> 8);
	cmd[1] = (uint8_t)(CMD_MEASURE_TEMP & 0xFF);
	HAL_I2C_Master_Transmit(&hi2c1, SHT31_ADDR, cmd, 2, 1000);
	HAL_Delay(50);
	// Read temperature data
	HAL_I2C_Master_Receive(&hi2c1, SHT31_ADDR, data, 3, 1000);
	temp_raw = data[0] << 8 | data[1];
	*temperature = ((float)temp_raw * 175.0f / 65535.0f) - 45.0f;

	// Send command to measure humidity
	cmd[0] = (uint8_t)(CMD_MEASURE_HUMIDITY >> 8);
	cmd[1] = (uint8_t)(CMD_MEASURE_HUMIDITY & 0xFF);
	HAL_I2C_Master_Transmit(&hi2c1, SHT31_ADDR, cmd, 2, 1000);
	HAL_Delay(50);
	// Read humidity data
	HAL_I2C_Master_Receive(&hi2c1, SHT31_ADDR, data, 3, 1000);
	humidity_raw = data[0] << 8 | data[1];
	*humidity = ((float)humidity_raw * 100.0f / 65535.0f);
}
