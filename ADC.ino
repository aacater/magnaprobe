#include <SPI.h>
#include "AD7791.h"

#define CS_PIN   4  
#define SCK_PIN  18  
#define MISO_PIN 19  
#define MOSI_PIN 23  


void setup() {
  Serial.begin(115200);
  delay(1000);


  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);


  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, CS_PIN);
  SPI.setDataMode(SPI_MODE3);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV16);


  Serial.println("AD7791 Communication Test");
  Serial.println("-------------------------");


  // Read Status Register
  digitalWrite(CS_PIN, LOW);
  SPI.transfer(0x08);                 // Request status register
  byte status = SPI.transfer(0x00);  // Read response
  digitalWrite(CS_PIN, HIGH);


  Serial.print("Status Register: 0x");
  Serial.println(status, HEX);


  if (status == 0x8C) {
    Serial.println("SUCCESS! AD7791 communicating correctly.");
  } else {
    Serial.println("FAIL. Check wiring.");
    Serial.println("Expected: 0x8C");
  }
}


void loop() {}

