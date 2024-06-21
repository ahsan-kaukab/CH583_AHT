/*
 * AHT21.h
 *
 *  Created on: 12-Jun-2023
 *      Author: sunil
 */

#ifndef INC_AHT21_H_
#define INC_AHT21_H_

#include "stdint.h"

typedef enum
{
  HAL_OK       = 0x00U,
  HAL_ERROR    = 0x01U,
  HAL_BUSY     = 0x02U,
  HAL_TIMEOUT  = 0x03U
} HAL_StatusTypeDef;

//extern I2C_HandleTypeDef hi2c1;
//#define AHT21_I2C_PORT hi2c1

HAL_StatusTypeDef AHT21_init(void);
uint32_t AHT21_Read_Humidity(void);
int32_t AHT21_Read_Temperature(void);

#endif /* INC_AHT21_H_ */
