/*
 * sht.h
 *
 *  Created on: Jul 24, 2026
 *      Author: HAIKAL
 */

#ifndef INC_SHT_H_
#define INC_SHT_H_

#include "main.h"

#define SHT31_ADDR             (0x44 << 1)

#define CMD_MEASURE_TEMP       0x2C06
#define CMD_MEASURE_HUMIDITY   0x2C10

void SHT31_ReadTempHumidity(float *temperature, float *humidity);

#endif /* INC_SHT_H_ */
