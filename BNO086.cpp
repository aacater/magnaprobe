/*
  BNO086.cpp
  by Alex Cater
  Based on work by Nathan Seidle
*/

#include "BNO086.h"

BNO08x imu;

void setupIMU(void) {
  //if (imu.begin() == false) {  // Setup without INT/RST control (Not Recommended)
  if (imu.begin(BNO08X_ADDR, Wire, BNO08X_INT, BNO08X_RST)) {
    Serial.println("BNO08x found!");
  } else {
    Serial.println("BNO08x not detected at default I2C address.");
  }
  
  Wire.setClock(400000); //Increase I2C data rate to 400kHz

  setIMUReports();

  Serial.println("Reading events");
  delay(100);
}

void enableMagnetometer(void) {
  if (imu.enableMagnetometer()) {
    Serial.println(F("Magnetometer enabled."));
    Serial.println(F("Output in form x, y, z, in uTesla"));
  } else {
    Serial.print("Could not enable magnetometer reporting at ");
    Serial.println(BNO08X_ADDR, HEX);
  }
}

void enableGyro(void) {
  if (imu.enableGyroIntegratedRotationVector()) {
    Serial.println(F("Gryo Integrated Rotation vector enabled"));
    Serial.println(F("Output in form i, j, k, real, gyroX, gyroY, gyroZ"));
  } else {
    Serial.println("Could not enable gyro integrated rotation vector");
  }
}

void enableGeomagneticRotationVector(void) {
  if (imu.enableGeomagneticRotationVector()) {
    Serial.println(F("Gryo Integrated Rotation vector enabled"));
    Serial.println(F("Output in form i, j, k, real, gyroX, gyroY, gyroZ"));
  } else {
    Serial.println("Could not enable gyro integrated rotation vector");
  }
}

void setIMUReports(void) {
  // enableMagnetometer();
  // enableGyro();
  enableGeomagneticRotationVector();
}

void getIMUData() {
  delay(10);

  if (imu.wasReset()) {
    Serial.print("sensor was reset ");
    setIMUReports();
  }

  if (imu.getSensorEvent()) {
    switch(imu.getSensorEventID()) {
      case SENSOR_REPORTID_MAGNETIC_FIELD: {
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
        break;
      }

      case SENSOR_REPORTID_GYRO_INTEGRATED_ROTATION_VECTOR: {
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
        break;
      }

      case SENSOR_REPORTID_GEOMAGNETIC_ROTATION_VECTOR: {
        float roll = (imu.getRoll()) * 180.0 / PI; // Convert roll to degrees
        float pitch = (imu.getPitch()) * 180.0 / PI; // Convert pitch to degrees
        float yaw = (imu.getYaw()) * 180.0 / PI; // Convert yaw / heading to degrees

        Serial.print(roll, 1);
        Serial.print(F(","));
        Serial.print(pitch, 1);
        Serial.print(F(","));
        Serial.print(yaw, 1);

        Serial.println();
        break;
      }
    }
  }
}
