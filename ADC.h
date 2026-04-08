/*
  ADC.h
*/

#ifndef ADC_h
#define ADC_h

#include <Arduino.h>
#include <SPI.h>
#include "AD7791.h"

#define ADC_CS_PIN   8
#define ADC_SCK_PIN  18
#define ADC_MISO_PIN 19
#define ADC_MOSI_PIN 23

uint32_t readADCData(void);
void setupADC(void);

#endif
