#ifndef _NTC_H
#define _NTC_H

/*
  Author:     Nima Askari
  WebSite:    http://www.github.com/NimaLTD
  Instagram:  http://instagram.com/github.NimaLTD
  Youtube:    https://www.youtube.com/channel/UCUhY7qY1klJm1d2kulr9ckw

  Version:    1.0.0


  Reversion History:

  (1.0.0)
  First release.
*/

#include <stdint.h>

uint32_t ntc_convertToC(uint16_t adcValue);
uint32_t ntc_convertToF(uint16_t adcValue);

#endif
