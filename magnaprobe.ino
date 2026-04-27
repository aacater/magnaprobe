/*
  magnaprobe.h
*/

// #include "sdcard.ino"
#include "BNO086.ino"
#include "ADC.ino"
#include "sdcard.h"

MagnaSD sd;

void setup(void) {
  Serial.begin(115200);
  Wire.begin();
  
  setupIMU();
  setupACD();
  sd.SDset();
}

void loop(void) {
  sd.record();
}
