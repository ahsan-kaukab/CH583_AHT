#include <stdio.h>
#include <stdint.h>
#include "AHT.h"
#include "app_i2c.h"

static int AHT1x_SENSOR = 0x00;
static AHTXX_I2C_SENSOR _sensorType;
static uint8_t _address;
static uint8_t _status;

uint8_t begin(int32_t sda, int32_t scl, uint32_t speed, uint32_t stretch) //"int32_t" for Master SDA & SCL, "uint8_t" for Slave SDA & SCL
{
	_address = AHTXX_ADDRESS_X38;
	i2c_app_init(_address);
	mDelaymS(AHT2X_POWER_ON_DELAY); //wait for sensor to initialize
	return softReset();                                      //soft reset is recommended at start (reset, set normal mode, set calibration bit & check calibration bit)
}


/**************************************************************************/
/*
    readHumidity()

    Read relative humidity, in %

    NOTE:
    - relative humidity range........ 0%..100%
    - relative humidity resolution... 0.024%
    - relative humidity accuracy..... +-2%
    - response time............ 5..30sec
    - measurement with high frequency leads to heating of the
      sensor, must be > 2 seconds apart to keep self-heating below 0.1C
    - long-term exposure for 60 hours outside the normal range
      (humidity > 80%) can lead to a temporary drift of the
      signal +3%, sensor slowly returns to the calibrated state at normal
      operating conditions

    - sensors data structure:
      - {status, RH, RH, RH+T, T, T, CRC*}, *CRC for AHT2x only

    - normal operating range T -20C..+60C, RH 10%..80%
    - maximum operating rage T -40C..+80C, RH 0%..100%
*/
/**************************************************************************/
float readHumidity(uint8_t readAHT)
{
  if (readAHT == AHTXX_FORCE_READ_DATA) {_readMeasurement();} //force to read data via I2C & update "_rawData[]" buffer
  if (_status != AHTXX_NO_ERROR)        {return AHTXX_ERROR;} //no reason to continue, call "getStatus()" for error description

  uint32_t humidity   = _rawData[1];                          //20-bit raw humidity data
           humidity <<= 8;
           humidity  |= _rawData[2];
           humidity <<= 4;
           humidity  |= _rawData[3] >> 4;

  if (humidity > 0x100000) {humidity = 0x100000;}             //check if RH>100, no need to check for RH<0 since "humidity" is "uint"

  return ((float)humidity / 0x100000) * 100;
}


/**************************************************************************/
/*
    readTemperature()

    Read temperature, in C 

    NOTE:
    - temperature range........ -40C..+85C
    - temperature resolution... 0.01C
    - temperature accuracy..... +-0.3C
    - response time............ 5..30sec
    - measurement with high frequency leads to heating of the
      sensor, must be > 2 seconds apart to keep self-heating below 0.1C

    - sensors data structure:
      - {status, RH, RH, RH+T, T, T, CRC*}, *CRC for AHT2x only
*/
/**************************************************************************/
float readTemperature(uint8_t readAHT)
{
  if (readAHT == AHTXX_FORCE_READ_DATA) {_readMeasurement();} //force to read data via I2C & update "_rawData[]" buffer
  if (_status != AHTXX_NO_ERROR)        {return AHTXX_ERROR;} //no reason to continue, call "getStatus()" for error description

  uint32_t temperature   = _rawData[3] & 0x0F;                //20-bit raw temperature data
           temperature <<= 8;
           temperature  |= _rawData[4];
           temperature <<= 8;
           temperature  |= _rawData[5];

  return ((float)temperature / 0x100000) * 200 - 50;
}


/**************************************************************************/
/*
    setNormalMode()  
 
    Set normal measurement mode

    NOTE:
    - no info in datasheet, suspect this is one measurement & power down
    - true=success, false=I2C error
*/
/**************************************************************************/
uint8_t setNormalMode()
{
  return _setInitializationRegister(AHTXX_INIT_CTRL_CAL_ON | AHT1X_INIT_CTRL_NORMAL_MODE);
}


/**************************************************************************/
/*
    setCycleMode()  
 
    Set cycle measurement mode

    NOTE:
    - no info in datasheet, suspect this is continuous measurement
    - true=success, false=I2C error
*/
/**************************************************************************/
uint8_t setCycleMode()
{
  return _setInitializationRegister(AHTXX_INIT_CTRL_CAL_ON | AHT1X_INIT_CTRL_CYCLE_MODE);
}


/**************************************************************************/
/*
    setComandMode()  
 
    Set command measurement mode

    NOTE:
    - no info in datasheet
    - true=success, false=I2C error
*/
/**************************************************************************/
uint8_t setComandMode()
{
  return _setInitializationRegister(AHTXX_INIT_CTRL_CAL_ON | AHT1X_INIT_CTRL_CMD_MODE);
}


/**************************************************************************/
/*
    softReset()  
 
    Restart sensor, without power off

    NOTE:
    - takes 20ms
    - all registers set to default
*/
/**************************************************************************/
uint8_t softReset()
{
	i2c_write_to(_address,AHTXX_SOFT_RESET_REG,1,10,1);
	mDelaymS(AHTXX_SOFT_RESET_DELAY);

	return ((setNormalMode() == true) && (_getCalibration() == AHTXX_STATUS_CTRL_CAL_ON)); //set mode & check calibration bit
	return 0;
}


/**************************************************************************/
/*
    getStatus()  
 
    Return sensor status

    NOTE:
    - returned statuse:
      - AHTXX_NO_ERROR   = 0x00, success, no errors
      - AHTXX_BUSY_ERROR = 0x01, sensor is busy
      - AHTXX_ACK_ERROR  = 0x02, sensor didn't return ACK
      - AHTXX_DATA_ERROR = 0x03, received data smaller than expected
      - AHTXX_CRC8_ERROR = 0x04, computed CRC8 not match received CRC8, for AHT2x only
*/
/**************************************************************************/
uint8_t getStatus()
{
  return _status;
}

/**************************************************************************/
/*
    setType()  
 
    Set sensor type on the fly

    NOTE:
    - AHT1x vs AHT2x:
      - AHT1x +1.8v..+3.6v, AHT2x 2.2v..5.5v
      - AHT1x 0.25uA..320uA, AHT2x 0.25uA..980uA
      - AHT2x support CRC8 check
*/
/**************************************************************************/
void setType(AHTXX_I2C_SENSOR sensorType)
{
  _sensorType = sensorType;
}

/**************************************************************************/
/*
    _readMeasurement()

    Start new measurement, read sensor data to buffer & collect errors

    NOTE:
    - sensors data structure:
      - {status, RH, RH, RH+T, T, T, CRC*}, *CRC for AHT2x only & for
        status description see "_readStatusRegister()" NOTE
*/
/**************************************************************************/
void _readMeasurement()
{

	//	i2c_write_to(_address,AHTXX_STATUS_REG,1,10,1);
	//	i2c_read_from(_address,_rawData,(uint8_t)1,10,1);
	/* send measurement command */

	i2c_write_to(_address,AHTXX_START_MEASUREMENT_REG,1,10,1);
	i2c_write_to(_address,AHTXX_START_MEASUREMENT_CTRL,1,10,1);
	i2c_write_to(_address,AHTXX_START_MEASUREMENT_CTRL_NOP,1,10,1);

	/* check busy bit */
	_status = _getBusy(AHTXX_FORCE_READ_DATA);                                                //update status byte, read status byte & check busy bit

	if      (_status == AHTXX_BUSY_ERROR) {mDelaymS(AHTXX_MEASUREMENT_DELAY - AHTXX_CMD_DELAY);}
	else if (_status != AHTXX_NO_ERROR)   {return;}                                           //no reason to continue, received data smaller than expected

	/* read data from sensor */
	uint8_t dataSize;

	if   (_sensorType == AHT1x_SENSOR) {dataSize = 6;}   //{status, RH, RH, RH+T, T, T, CRC*}, *CRC for AHT2x only
	else                               {dataSize = 7;}

	i2c_read_from(_address,_rawData,dataSize,10,1);

	/* check busy bit after measurement dalay */
	_status = _getBusy(AHTXX_USE_READ_DATA);             //update status byte, read status byte & check busy bit

	if (_status != AHTXX_NO_ERROR) {return;}             //no reason to continue, sensor is busy

	/* check CRC8, for AHT2x only */
	if ((_sensorType == AHT2x_SENSOR) && (_checkCRC8() != true)) {_status = AHTXX_CRC8_ERROR;} //update status byte
}

/**************************************************************************/
/*
    _setInitializationRegister()
 
    Set initialization register

    NOTE:
    - true=success, false=I2C error
*/
/**************************************************************************/
uint8_t _setInitializationRegister(uint8_t value)
{
	mDelaymS(AHTXX_CMD_DELAY);
	if   (_sensorType == AHT1x_SENSOR) {	i2c_write_to(_address,AHT1X_INIT_REG,1,10,1);} //send initialization command, for AHT1x only
	else                               {	i2c_write_to(_address,AHT2X_INIT_REG,1,10,1);} //send initialization command, for AHT2x only

	i2c_write_to(_address,value,1,10,1);
	i2c_write_to(_address,AHTXX_INIT_CTRL_NOP,1,10,1);
	return 0;
}

/**************************************************************************/
/*
    _readStatusRegister()

    Read status register

    NOTE:
    - AHT1x status register controls:
      7    6    5    4   3    2   1   0
      BSY, MOD, MOD, xx, CAL, xx, xx, xx
      - BSY:
        - 1, sensor busy/measuring
        - 0, sensor idle/sleeping
      - MOD:
        - 00, normal mode
        - 01, cycle mode
        - 1x, comand mode
      - CAL:
        - 1, calibration on
        - 0, calibration off

    - AHT2x status register controls:
      7    6   5   4   3    2   1  0
      BSY, xx, xx, xx, CAL, xx, xx, xx

    - under normal conditions status is 0x18 & 0x80 if the sensor is busy
*/
/**************************************************************************/
uint8_t _readStatusRegister()
{
	uint8_t data;
	mDelaymS(AHTXX_CMD_DELAY);

	i2c_write_to(_address,AHTXX_STATUS_REG,1,10,1);
	i2c_read_from(_address,_rawData,(uint8_t)1,10,1);

	return _rawData[0];
}

/**************************************************************************/
/*
    _getCalibration()

    Read calibration bits from status register

    NOTE:
    - 0x08=loaded, 0x00=not loaded, 0xFF=I2C error
    - calibration status check should only be performed at power-up,
      rechecking is not required during data collection
*/
/**************************************************************************/
uint8_t _getCalibration()
{
  uint8_t value = _readStatusRegister();

  if (value != AHTXX_ERROR) {return (value & AHTXX_STATUS_CTRL_CAL_ON);} //0x08=loaded, 0x00=not loaded
                             return AHTXX_ERROR;                         //collision on I2C bus, sensor didn't return ACK
}

/**************************************************************************/
/*
    _getBusy()

    Read/check busy bit after measurement command

    NOTE:
    - part of "readRawMeasurement()" function!!!
    - 0x80=busy, 0x00=measurement completed, etc
*/
/**************************************************************************/
uint8_t _getBusy(uint8_t readAHT)
{
	  if (readAHT == AHTXX_FORCE_READ_DATA)                    //force to read data via I2C & update "_rawData[]" buffer
	  {
		mDelaymS(AHTXX_CMD_DELAY);
		i2c_read_from(_address,_rawData,(uint8_t)1,1,10);                        //read 1-byte from "wire.h" rxBuffer
	  }
	  return 0;
//	  if   ((_rawData[0] & AHTXX_STATUS_CTRL_BUSY) == AHTXX_STATUS_CTRL_BUSY) {_status = AHTXX_BUSY_ERROR;} //0x80=busy, 0x00=measurement completed
//	  else                                                                    {_status = AHTXX_NO_ERROR;}
//
//	  return _status;
}

/**************************************************************************/
/*
    _checkCRC8()

    Check CRC-8-Maxim of AHT2X measured data

    NOTE:
    - part of "readRawMeasurement()" function!!!
    - only AHT2x sensors have CRC
    - initial value=0xFF, polynomial=(x8 + x5 + x4 + 1) ie 0x31 CRC [7:0] = 1+X4+X5+X8
*/
/**************************************************************************/
uint8_t _checkCRC8()
{
  if (_sensorType == AHT2x_SENSOR)
  {
    uint8_t crc = 0xFF;                                      //initial value

    for (uint8_t byteIndex = 0; byteIndex < 6; byteIndex ++) //6-bytes in data, {status, RH, RH, RH+T, T, T, CRC}
    {
      crc ^= _rawData[byteIndex];

      for(uint8_t bitIndex = 8; bitIndex > 0; --bitIndex)    //8-bits in byte
      {
        if   (crc & 0x80) {crc = (crc << 1) ^ 0x31;}         //0x31=CRC seed/polynomial 
        else              {crc = (crc << 1);}
      }
    }

    return (crc == _rawData[6]);
  }

  return 1;
}
