
#include "ntc.h"
#include "ntcConfig.h"
#include <math.h>

double custom_log(double x) {
    if (x <= 0.0) {
        // Handle invalid input (logarithm of non-positive number)
        return -1.0; // Or handle differently based on your application's needs
    }

    double result = 0.0;
    double term = (x - 1) / (x + 1);
    double term_squared = term * term;
    double denom = 1.0;
    double frac = term;
    int n = 1;

    while (frac != 0) {
        result += frac / denom;
        denom += 2.0;
        frac *= term_squared;
        n++;
    }

    return 2.0 * result;
}

//#######################################################################################
uint32_t ntc_convertToC(uint16_t adcValue)
{
  float rntc = (float)_NTC_R_SERIES / (((float)_NTC_ADC_MAX / (float)adcValue ) - 1.0f);
  uint32_t temp;
  temp = rntc / (float)_NTC_R_NOMINAL;
  temp = custom_log(temp);
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
  temp = custom_log(temp);
  temp /= (float)_NTC_BETA;
  temp += 1.0f / ((float)_NTC_TEMP_NOMINAL + 273.15f);
  temp = 1.0f / temp;
  temp -= 273.15f;
  return (temp * 9.0f / 5.0f) + 32.f;
}
//#######################################################################################
