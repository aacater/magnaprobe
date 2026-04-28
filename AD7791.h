/*
  AD7791.h
  Source: https://github.com/analogdevicesinc/arduino/blob/d5946b9d890ca4319f12887ea6cef78a6a872af7/Arduino%20Uno%20R3/examples/CN0216_example/AD7791.h
*/
#ifndef AD7791_h
#define AD7791_h

#include <Arduino.h>

/*****************************************************************************/
/************************** ADC Address Definitions **************************/
/*****************************************************************************/

//ADC Write Commands
#define MODE_WRITE			0x10					// Write to the Mode Register
#define FILTER_WRITE		0x20					// Write to the Filter Register

//ADC Read Commands
#define STATUS_READ			0x08					// Read from the Status Register
#define MODE_READ			  0x18					// Read from the Mode Register
#define FILTER_READ			0x28					// Read from the Filter Register
#define DATA_READ			  0x38					// Read from the Data Register

#define RESET				    0xFF					// Resets the chip to default

//Pins
#define AD7791_SS				4				// AD7791 SPI chip select

class AD7791class {
  public:
    void writeAd7791 (uint8_t ui8address, uint8_t ui8value);
    uint32_t readAd7791 (uint8_t ui8address);
    void AD7791_SPI_Configuration(void);
  private:
};

extern AD7791class AD7791;

#endif
