/*
  sdcard.h
  by Ryuma May
*/

#ifndef sdcard_h
#define sdcard_h

#include <Arduino.h>

#include <SPI.h>
#include <SD.h>
#include <FS.h>
#include <TinyGPS++.h>

#define SD_CS 5
#define BUTTON_PIN 15 //button
#define BUZZER_PIN 27 //buzzer
#define GPS_RX 16 //GPIO 16
#define GPS_TX 17 //GPIO 17
#define CS_PIN 5 //ADC

// Data structure
struct DataRecord {
  char dateStr[11];
  int year;
  int month;
  int day;

  char timeStr[9];
  int hour;
  int minute;
  int second;

  double latitude;       // GPS latitude or X coordinate
  double longitude;      // GPS longitude or Y coordinate
  double altitude;       // height above sea level or sensor origin

  float temperature;
  float depth;
  float angle;
  float REALdepth;
};


class MagnaSD {
public:
  void SDset();
  void record();

private:
  File MagnaFile;
  TinyGPSPlus gps;
  HardwareSerial GPS = HardwareSerial(2);

  bool recorded = false;
  unsigned long lastLog = 0;
  const unsigned long dt = 1000;

  void createMagnaFile();
  void writeRecord(DataRecord d);
  bool allDataMeasured(DataRecord d);
};

#endif
