/*
  magnaprobe.h
*/

// #include "sdcard.ino"
#include "BNO086.ino"
#include "ADC.ino"

void setup(void) {
  Serial.begin(115200);
  Wire.begin();
  
  // setupSD();
  setupIMU();
  setupACD();
}

void loop(void) {

}