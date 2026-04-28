/*
  sdcard2.cpp
  by Ryuma May
*/

#include "sdcard.h"
#include <math.h>
#include "AD7791.h"

extern float currentDepth;

void MagnaSD::SDset() {
  Serial.begin(115200);
  GPS.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX); //seting GPS

  pinMode(BUTTON_PIN, INPUT_PULLUP); //button setting
  pinMode(BUZZER_PIN, OUTPUT); //buzzer setting

  /*SPI.begin(18, 19, 23); //AD7791 setup clock,MISO,MOSI
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);*/

  //read and SDcard 
  while (!SD.begin(SD_CS)){
    Serial.println("No SD card attached.");
    
    digitalWrite(BUZZER_PIN, HIGH);  // buzzer ON
    delay(600);
    digitalWrite(BUZZER_PIN, LOW);
    delay(500);
  }

  digitalWrite(BUZZER_PIN, HIGH);  // buzzer ON
  delay(50);
  digitalWrite(BUZZER_PIN, LOW);  // buzzer ON
  delay(10);
  digitalWrite(BUZZER_PIN, HIGH);  // buzzer ON
  delay(50);
  digitalWrite(BUZZER_PIN, LOW);  // buzzer ON

  createMagnaFile();
}

// create normal file
void MagnaSD::createMagnaFile() {
  if (!SD.exists("/Magnaprobe")) {
    SD.mkdir("/Magnaprobe");
    //Serial.println("File create");
  }
  //Serial.println("File exist");
}

//write record
void MagnaSD::writeRecord(DataRecord d) {

  String filename = "/Magnaprobe/" + String(d.dateStr) + ".csv";

  MagnaFile = SD.open(filename, FILE_APPEND);

    if (!MagnaFile) {
      Serial.println("Failed to create file");
      digitalWrite(BUZZER_PIN, HIGH);  // buzzer ON
      delay(600);
      digitalWrite(BUZZER_PIN, LOW);
      delay(500);
      return;
    }
    else{
      Serial.println(".csv created"); 
    }
    
    while (MagnaFile.size() == 0) {
      MagnaFile.println("#MagnaFile:");
      MagnaFile.print("\n");
      MagnaFile.println("Date,Time,latitude,longitude,altitude,temperature,Measure_Depth,Measure_Angle,Real_depth");
      MagnaFile.flush();

    }
    

    if (!MagnaFile) return;
    
  //For recording to scv.
  MagnaFile.print(d.dateStr);
  MagnaFile.print(",");
  MagnaFile.print(d.timeStr);
  MagnaFile.print(",");
  MagnaFile.print(d.latitude,6);
  MagnaFile.print(",");
  MagnaFile.print(d.longitude,6);
  MagnaFile.print(",");
  MagnaFile.print(d.altitude, 3);
  MagnaFile.print(",");
  MagnaFile.print(d.temperature);
  MagnaFile.print(",");
  MagnaFile.print(d.depth);
  MagnaFile.print(",");
  MagnaFile.print(d.angle);
  MagnaFile.print(",");
  MagnaFile.println(d.REALdepth);

   //For recording to scv.
   Serial.print(d.dateStr);
   Serial.print(",");
   Serial.print(d.timeStr);
   Serial.print(",");
   Serial.print(d.latitude,6); // GPS latitude or X coordinate in 6decimal
   Serial.print(",");
   Serial.print(d.longitude,6); // GPS longitude or Y coordinate in 6decimal
   Serial.print(",");
   Serial.print(d.altitude, 3);
   Serial.print(",");
   Serial.print(d.temperature);
   Serial.print(",");
   Serial.print(d.depth);
   Serial.print(",");
   Serial.print(d.angle);
   Serial.print(",");
   Serial.println(d.REALdepth);

   MagnaFile.flush();
   MagnaFile.close();
}

//for GPS
bool MagnaSD::allDataMeasured(DataRecord d) {
  if (d.latitude == 0 || d.longitude == 0){
    return false;
  }
  if (d.depth < 0.0) {
    return false;
  }
  if (d.angle < 0) {
    return false;
  }
  if (d.altitude == 0){
    return false;
  }
  return true;
}

void MagnaSD::record() {
 while(GPS.available() > 0) {
  gps.encode(GPS.read());
 }

 if (!recorded && digitalRead(BUTTON_PIN) == LOW) {

    if (millis() - lastLog >= dt){
     lastLog = millis();
     
     DataRecord data;
     
     if(gps.time.isValid() && gps.date.isValid()) { //Time and Date
       data.year = gps.date.year();
       data.month = gps.date.month();
       data.day = gps.date.day();

       data.hour = gps.time.hour();
       data.minute = gps.time.minute();
       data.second = gps.time.second();

       sprintf(data.dateStr, "%04d-%02d-%02d",  data.year, data.month, data.day);
       sprintf(data.timeStr, "%02d:%02d.%02d", data.hour, data.minute, data.second);
      }
      else {
        data.year  = 2026;  // 0–9999
        data.month = 4; // 1–12
        data.day   = 27; // 1–31

        sprintf(data.dateStr, "%04d-%02d-%02d",  data.year, data.month, data.day);
        //sprintf(data.timeStr, "2026-04-27");
        sprintf(data.timeStr, "**:**.**");
      }
      //GPS
      if (gps.location.isValid()) {
       data.latitude = gps.location.lat();
       data.longitude = gps.location.lng();
      } 
      /*else{
        data.latitude = millis();
       data.longitude = millis();
      }*/
      if (gps.altitude.isValid()) {
       data.altitude = gps.altitude.meters();
      }
      /*else {
        data.altitude = millis();
      }*/

      //data.depth = readDepth();
      data.depth = currentDepth; //currentDepth = depth drag from AD991.h
      
      // need to replace to the actual measurement
      data.temperature = -40; //<- if want add temerature device to record
      data.angle = 0; //<- change to the 

      data.REALdepth = data.depth*cos(data.angle);
     
     if (allDataMeasured(data)) {
          writeRecord(data);
          recorded = true;
          digitalWrite(BUZZER_PIN, LOW);
      }
   }
      // if not elif, buzzer while held
     if (digitalRead(BUTTON_PIN) == LOW) {
        digitalWrite(BUZZER_PIN, HIGH);  // buzz while held
      }
    } 
    else {
        digitalWrite(BUZZER_PIN, LOW);
    }
//if instead of while — non blocking
    if (digitalRead(BUTTON_PIN) == HIGH) {
      recorded = false;
    }
}
