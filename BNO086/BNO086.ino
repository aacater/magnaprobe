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

#include <Wire.h>

// http://librarymanager/All#SparkFun_BNO08x
#include "SparkFun_BNO08x_Arduino_Library.h"

BNO08x imu;

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

void setup() {
  Serial.begin(115200);
  
  Serial.println();
  Serial.println("BNO08x Read Example");

  Wire.begin();

  //if (imu.begin() == false) {  // Setup without INT/RST control (Not Recommended)
  if (imu.begin(BNO08X_ADDR, Wire, BNO08X_INT, BNO08X_RST) == false) {
    Serial.println("BNO08x not detected at default I2C address. Check your jumpers and the hookup guide. Freezing...");
    while (1)
      ;
  }
  Serial.println("BNO08x found!");

  Wire.setClock(400000); //Increase I2C data rate to 400kHz

  setReports();

  Serial.println("Reading events");
  delay(100);
}

void setReports(void) {
  Serial.println("Setting desired reports");
  if (imu.enableMagnetometer()) {
    Serial.println(F("Magnetometer enabled"));
    Serial.println(F("Output in form x, y, z, in uTesla"));
  } else {
    Serial.println("Could not enable magnetometer");
  }
  if (imu.enableGyroIntegratedRotationVector()) {
    Serial.println(F("Gryo Integrated Rotation vector enabled"));
    Serial.println(F("Output in form i, j, k, real, gyroX, gyroY, gyroZ"));
  } else {
    Serial.println("Could not enable gyro integrated rotation vector");
  }
}

void loop() {
  delay(10);

  if (imu.wasReset()) {
    Serial.print("sensor was reset ");
    setReports();
  }

  if (imu.getSensorEvent()) {

    if (imu.getSensorEventID() == SENSOR_REPORTID_MAGNETIC_FIELD) {

      float x = imu.getMagX();
      float y = imu.getMagY();
      float z = imu.getMagZ();
      byte accuracy = imu.getMagAccuracy();

      Serial.print(x, 2);
      Serial.print(F(","));
      Serial.print(y, 2);
      Serial.print(F(","));
      Serial.print(z, 2);
      Serial.print(F(","));
      Serial.print(accuracy, 2);

      Serial.println();
    }
    else if (imu.getSensorEventID() == SENSOR_REPORTID_GYRO_INTEGRATED_ROTATION_VECTOR) {

    float RVI = imu.getGyroIntegratedRVI();
    float RVJ = imu.getGyroIntegratedRVJ();
    float RVK = imu.getGyroIntegratedRVK();
    float RVReal = imu.getGyroIntegratedRVReal();
    float gyroX = imu.getGyroIntegratedRVangVelX();
    float gyroY = imu.getGyroIntegratedRVangVelY();
    float gyroZ = imu.getGyroIntegratedRVangVelZ();

    Serial.print(RVI, 2);
    Serial.print(F(","));
    Serial.print(RVJ, 2);
    Serial.print(F(","));
    Serial.print(RVK, 2);
    Serial.print(F(","));
    Serial.print(RVReal, 2);
    Serial.print(F(","));
    Serial.print(gyroX, 2);
    Serial.print(F(","));
    Serial.print(gyroY, 2);
    Serial.print(F(","));
    Serial.print(gyroZ, 2);

    Serial.println();
    }
  }
}
