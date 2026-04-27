/*
  BNO086.h
*/

/*
  Hardware Connections:
  IoT RedBoard --> BNO08x
  QWIIC --> QWIIC
  A4  --> INT
  A5  --> RST

  BNO08x "mode" jumpers set for I2C (default):
  PSO: OPEN
  PS1: OPEN
*/

#ifndef BNO086_h
#define BNO086_h

#include <Wire.h>

// http://librarymanager/All#SparkFun_BNO08x
#include "SparkFun_BNO08x_Arduino_Library.h"

// For the most reliable interaction with the SHTP bus, we need
// to use hardware reset control, and to monitor the H_INT pin.
// The H_INT pin will go low when its okay to talk on the SHTP bus.
// Note, these can be other GPIO if you like.
// Define as -1 to disable these features.

//#define BNO08X_INT  A4
#define BNO08X_INT  -1
//#define BNO08X_RST  A5
#define BNO08X_RST  -1

#define BNO08X_ADDR 0x4B

extern BNO08x imu;

void setupIMU(void);
void enableMagnetometer(void);
void enableGyro(void);
void enableGeomagneticRotationVector(void);
void setIMUReports(void);
void getIMUData(void);

#endif
