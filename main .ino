#include "AD7791.h"

AD7791class ADC;

void setup() {
    ADC.ADCsetup();
}

void loop() {
    ADC.ADCrecord();
    yield();
}
