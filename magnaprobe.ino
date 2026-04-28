/*
  magnaprobe.ino
  by Alex Cater, Griffin Marson, Ryuma May
*/

#include "BNO086.h"
#include "ADC.h"
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
  getIMUData();
  sd.record();
}
