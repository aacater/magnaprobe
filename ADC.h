#ifndef AD7791_h
#define AD7791_h

#include <Arduino.h>

#define MODE_WRITE    0x10
#define FILTER_WRITE  0x20
#define STATUS_READ   0x08
#define MODE_READ     0x18
#define FILTER_READ   0x28
#define DATA_READ     0x38
#define RESET         0xFF
#define AD7791_SS     4

extern float currentDepth;

class AD7791class {
  public:
    void ADCsetup();
    void ADCrecord();
    void writeAd7791(uint8_t ui8address, uint8_t ui8value);
    uint32_t readAd7791(uint8_t ui8address);
    void AD7791_SPI_Configuration(void);
    float rawToDepth(uint32_t raw);
};

extern AD7791class AD7791;

#endif
