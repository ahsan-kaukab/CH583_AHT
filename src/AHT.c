/*
 * AHT21.c
 *
 *  Created on: 12-Jun-2023
 *      Author: sunil
 */

#include "AHT.h"
#include "CH58x_common.h"
#include "app_i2c.h"
#include <string.h>


//uint8_t AHT_21_ADDR = 0x38 << 1;
uint8_t AHT_21_ADDR = 0x38;
uint32_t i2c_RETRY_TIME = 100;

HAL_StatusTypeDef AHT21_init(void) {
	HAL_StatusTypeDef ret = 0;
	uint8_t buff[8];
	i2c_app_init(AHT_21_ADDR);
	/*ret = HAL_I2C_IsDeviceReady(&AHT21_I2C_PORT, AHT_21_ADDR, 2,
			i2c_RETRY_TIME); // is device avaialble ?
	if (ret != HAL_OK) {
		return HAL_ERROR;
	}*/
	buff[0] = 0x71;
	i2c_write_to(AHT_21_ADDR, buff, 1,200,1);
	/*ret = HAL_I2C_Master_Transmit(&AHT21_I2C_PORT, AHT_21_ADDR, buff, 1,
			i2c_RETRY_TIME);
	if (ret != HAL_OK) {
		return HAL_ERROR;
	}*/
	i2c_read_from(AHT_21_ADDR, buff, 1,1, 2000);
	//ret = HAL_I2C_Master_Receive(&AHT21_I2C_PORT, AHT_21_ADDR, buff, 1,
	//		i2c_RETRY_TIME);
	buff[0] = buff[0] & 0x18;
	if (buff[0] != 0x18) {
		return HAL_ERROR;
	}
	return ret;
}

uint32_t AHT21_Read_Humidity(void) {
	HAL_StatusTypeDef ret =0;
	uint8_t buff[8];
	uint32_t humidity;

	buff[0] = 0xAC;
	buff[1] = 0X33;
	buff[2] = 0x00;
	i2c_write_to(AHT_21_ADDR, buff, 3,200,1);
	//ret = HAL_I2C_Master_Transmit(&AHT21_I2C_PORT, AHT_21_ADDR, buff, 3,
	//		i2c_RETRY_TIME);
	if (ret != HAL_OK) {
		return HAL_ERROR;
	}
	mDelaymS(100);
	i2c_read_from(AHT_21_ADDR, buff, 6,1, 2000);
	//ret = HAL_I2C_Master_Receive(&AHT21_I2C_PORT, AHT_21_ADDR, buff, 6,
	//		i2c_RETRY_TIME);
	//CALCULATING HUMIDITY
	humidity = (buff[1] << 12) | (buff[2] << 4) | (buff[3] >> 4);
	humidity = (humidity * 100);
	humidity = humidity / 0x100000;
	return humidity;
}

int32_t AHT21_Read_Temperature(void) {
	HAL_StatusTypeDef ret;
	uint8_t buff[8];
	uint32_t temperature;

	buff[0] = 0xAC;
	buff[1] = 0X33;
	buff[2] = 0x00;
	i2c_write_to(AHT_21_ADDR, buff, 3,200,1);
	//ret = HAL_I2C_Master_Transmit(&AHT21_I2C_PORT, AHT_21_ADDR, buff, 3,
	//		i2c_RETRY_TIME);
//	if (ret != HAL_OK) {
//		return HAL_ERROR;
//	}
	mDelaymS(100);
	i2c_read_from(AHT_21_ADDR, buff, 6,1, 2000);
	//ret = HAL_I2C_Master_Receive(&AHT21_I2C_PORT, AHT_21_ADDR, buff, 6,
	//		i2c_RETRY_TIME);
	//CALCULATING TEMPERATURE
	temperature = ((buff[3] & 0xF) << 16) | (buff[4] << 8) | (buff[5]);
	temperature = (temperature * 200);
	temperature = temperature / 0x100000;
	temperature = temperature - 50;
	return temperature;
}
