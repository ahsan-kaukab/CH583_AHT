
#include "ntc.h"
#include "ntcConfig.h"
#include <math.h>

//#######################################################################################
uint32_t ntc_convertToC(uint16_t adcValue)
{
  float rntc = (float)_NTC_R_SERIES / (((float)_NTC_ADC_MAX / (float)adcValue ) - 1.0f);
  uint32_t temp;
  temp = rntc / (float)_NTC_R_NOMINAL;
  temp = logf(temp);
  temp /= (float)_NTC_BETA;
  temp += 1.0f / ((float)_NTC_TEMP_NOMINAL + 273.15f);
  temp = 1.0f / temp;
  temp -= 273.15f;
  return temp;
}
//#######################################################################################
uint32_t ntc_convertToF(uint16_t adcValue)
{
  float rntc = (float)_NTC_R_SERIES / (((float)_NTC_ADC_MAX / (float)adcValue ) - 1.0f);
  uint32_t temp;
  temp = rntc / (float)_NTC_R_NOMINAL;
  temp = logf(temp);
  temp /= (float)_NTC_BETA;
  temp += 1.0f / ((float)_NTC_TEMP_NOMINAL + 273.15f);
  temp = 1.0f / temp;
  temp -= 273.15f;
  return (temp * 9.0f / 5.0f) + 32.f;
}
//#######################################################################################
