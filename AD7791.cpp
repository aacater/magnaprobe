/*
  AD7791.cpp
*/

#include <Arduino.h>
#include <SPI.h>
#include "AD7791.h"

AD7791class AD7791;

#define DRDY_PIN      19
#define V_REF         3.3
#define DIVIDER_RATIO 3.0
#define V_SURFACE     7.7747
#define V_BOTTOM      0.0
#define STROKE_MM     1200.0
#define MODE_CONFIG   0x06
#define FILTER_CONFIG 0x04

float currentDepth = 0.0;

void AD7791class::AD7791_SPI_Configuration(void) {
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE3);
    SPI.setClockDivider(SPI_CLOCK_DIV128);
    delay(100);
}

uint32_t AD7791class::readAd7791(uint8_t ui8address) {
    uint8_t ui8AdcUpperCodes  = 0;
    uint8_t ui8AdcMiddleCodes = 0;
    uint8_t ui8AdcLowerCodes  = 0;
    uint32_t ui32AdcCodes     = 0;

    if (ui8address == DATA_READ) {
        digitalWrite(AD7791_SS, LOW);
        SPI.transfer(ui8address);
        ui8AdcUpperCodes  = SPI.transfer(0x00);
        ui8AdcMiddleCodes = SPI.transfer(0x00);
        ui8AdcLowerCodes  = SPI.transfer(0x00);
        digitalWrite(AD7791_SS, HIGH);
        ui32AdcCodes = ((long)ui8AdcUpperCodes << 16) |
                       ((long)ui8AdcMiddleCodes << 8)  |
                       ui8AdcLowerCodes;
    } else {
        digitalWrite(AD7791_SS, LOW);
        SPI.transfer(ui8address);
        ui8AdcLowerCodes = SPI.transfer(0x00);
        digitalWrite(AD7791_SS, HIGH);
        ui32AdcCodes = ui8AdcLowerCodes;
    }
    return ui32AdcCodes;
}

void AD7791class::writeAd7791(uint8_t ui8address, uint8_t ui8value) {
    if (ui8address != RESET) {
        digitalWrite(AD7791_SS, LOW);
        SPI.transfer(ui8address);
        SPI.transfer(ui8value);
        digitalWrite(AD7791_SS, HIGH);
    } else {
        digitalWrite(AD7791_SS, LOW);
        SPI.transfer(ui8value);
        SPI.transfer(ui8value);
        SPI.transfer(ui8value);
        SPI.transfer(ui8value);
        digitalWrite(AD7791_SS, HIGH);
    }
}

float AD7791class::rawToDepth(uint32_t raw) {
    float v_adc    = ((float)raw / 16777216.0) * V_REF;
    float v_sensor = v_adc * DIVIDER_RATIO;
    float depth    = ((v_sensor - V_BOTTOM) / (V_SURFACE - V_BOTTOM)) * STROKE_MM;
    if (depth < 0)         depth = 0;
    if (depth > STROKE_MM) depth = STROKE_MM;
    return depth;
}

void AD7791class::ADCsetup() {
    Serial.begin(115200);
    pinMode(AD7791_SS, OUTPUT);
    pinMode(DRDY_PIN, INPUT);
    digitalWrite(AD7791_SS, HIGH);

    SPI.begin(18, 19, 23, 4);
    AD7791.AD7791_SPI_Configuration();

    AD7791.writeAd7791(RESET, 0xFF);
    delay(10);
    AD7791.writeAd7791(MODE_WRITE,   MODE_CONFIG);
    delay(1);
    AD7791.writeAd7791(FILTER_WRITE, FILTER_CONFIG);
    delay(1);

    Serial.println("=== Magnaprobe Boot ===");
    Serial.print("Mode   (expect 0x06): 0x"); Serial.println(AD7791.readAd7791(MODE_READ),   HEX);
    Serial.print("Filter (expect 0x04): 0x"); Serial.println(AD7791.readAd7791(FILTER_READ), HEX);
    Serial.print("Status (expect 0x8C): 0x"); Serial.println(AD7791.readAd7791(STATUS_READ), HEX);
    Serial.println("=======================");
}

void AD7791class::ADCrecord() {
    if (digitalRead(DRDY_PIN) == HIGH) return;

    uint32_t raw = readAd7791(DATA_READ);
    float depth  = rawToDepth(raw);

    static float lastDepth = 0;
    currentDepth = (depth * 0.3) + (lastDepth * 0.7);
    lastDepth    = currentDepth;

    float v_adc    = ((float)raw / 16777216.0) * V_REF;
    float v_sensor = v_adc * DIVIDER_RATIO;

    Serial.print("Raw: ");         Serial.print(raw);
    Serial.print(" | V_Sensor: "); Serial.print(v_sensor, 4); Serial.print("V");
    Serial.print(" | Depth: ");    Serial.print(currentDepth, 1); Serial.println(" mm");
}
}
