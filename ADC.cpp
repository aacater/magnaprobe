/*
  ADC.cpp
  by Griffin Marson
*/

void setupADC(void) {
  pinMode(ADC_CS_PIN, OUTPUT);
  digitalWrite(ADC_CS_PIN, HIGH);

  SPI.begin(ADC_SCK_PIN, ADC_MISO_PIN, ADC_MOSI_PIN, ADC_CS_PIN);
	AD7791.AD7791_SPI_Configuration();

  Serial.println("AD7791 Communication Test");
  Serial.println("-------------------------");

  // Read Status Register
  if (AD7791.readAd7791(STATUS_READ) == 0x8C) {
    Serial.println("SUCCESS! AD7791 communicating correctly.");
  } else {
    Serial.println("FAIL. Check wiring.");
  }

	AD7791.writeAd7791 (RESET, 0xFF);		//Resets the part for initial use
	delay(1000);
	AD7791.writeAd7791 (MODE_WRITE, 0x00);	        //Mode register value (single conversion, +/- Vref input, unbuffered mode)
	AD7791.writeAd7791 (FILTER_WRITE, 0x07);	// Filter register value (clock not divided down, 9.5 Hz update rate)  
}

uint32_t readADCData(void) {
  uint32_t ADCStatus = 0;
	do {
    ADCStatus = AD7791.readAd7791(STATUS_READ);
  } while (ADCStatus & 0x80); // wait for RDY bit

  return AD7791.readAd7791(DATA_READ);
}

