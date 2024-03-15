#ifndef RTECHI2C_h
#define RTECHI2C_h

#include "Wire.h"

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define DEFAULT_SDA SDA;
#define DEFAULT_SCL SCL;


#define DEBUG_PRINTER Serial

#ifdef PCF8574_DEBUG
	#define DEBUG_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
	#define DEBUG_PRINTLN(...) { DEBUG_PRINTER.println(__VA_ARGS__); }
#else
	#define DEBUG_PRINT(...) {}
	#define DEBUG_PRINTLN(...) {}
#endif

#ifdef PCF8574_LOW_LATENCY
	#define READ_ELAPSED_TIME 0
#else
	#define READ_ELAPSED_TIME 10
#endif

#define P0  	0
#define P1  	1
#define P2  	2
#define P3  	3
#define P4  	4
#define P5  	5
#define P6  	6
#define P7  	7

#include <math.h>


class RTECHI2 {
public:

	RTECHI2(uint8_t address);
	RTECHI2(uint8_t address, uint8_t interruptPin,  void (*interruptFunction)() );

#if !defined(__AVR) && !defined(__STM32F1__) && !defined(TEENSYDUINO)
	RTECHI2(uint8_t address, uint8_t sda, uint8_t scl);
	RTECHI2(uint8_t address, uint8_t sda, uint8_t scl, uint8_t interruptPin,  void (*interruptFunction)());
#endif

#ifdef ESP32
	///// changes for second i2c bus
	RTECHI2(TwoWire *pWire, uint8_t address);
	RTECHI2(TwoWire *pWire, uint8_t address, uint8_t sda, uint8_t scl);

	RTECHI2(TwoWire *pWire, uint8_t address, uint8_t interruptPin,  void (*interruptFunction)() );
	RTECHI2(TwoWire *pWire, uint8_t address, uint8_t sda, uint8_t scl, uint8_t interruptPin,  void (*interruptFunction)());
#endif

	bool begin();
	void pinMode(uint8_t pin, uint8_t mode, uint8_t output_start = HIGH);

	bool digitalWrite(uint8_t pin, uint8_t value);

	uint8_t getTransmissionStatusCode() const {
		return transmissionStatus;
	}

	bool isLastTransmissionSuccess(){
		return transmissionStatus==0;
	}
private:
	uint8_t _address;

	#if !defined(DEFAULT_SDA)
	#  if defined(__STM32F1__)
	#    define DEFAULT_SDA PB7
	#  elif defined(ESP8266)
	#    define DEFAULT_SDA 4
	#  elif defined(SDA)
	#    define DEFAULT_SDA SDA
	#  else
	#    error "Error define DEFAULT_SDA, SDA not declared, if you have this error contact the mantainer"
	#  endif
	#endif
	#if !defined(DEFAULT_SCL)
	#  if defined(__STM32F1__)
	#    define DEFAULT_SCL PB6
	#  elif defined(ESP8266)
	#    define DEFAULT_SCL 5
	#  elif defined(SDA)
	#    define DEFAULT_SCL SCL
	#  else
	#    error "Error define DEFAULT_SCL, SCL not declared, if you have this error contact the mantainer"
	#  endif
	#endif

	uint8_t _sda = DEFAULT_SDA;
	uint8_t _scl = DEFAULT_SCL;

	TwoWire *_wire;

	bool _usingInterrupt = false;
	uint8_t _interruptPin = 2;
	void (*_interruptFunction)(){};

	byte writeMode 			= 	B00000000;
	byte writeModeUp		= 	B00000000;
	byte readMode 			= 	B00000000;
	byte readModePullUp 	= 	B00000000;
	byte readModePullDown 	= 	B00000000;
	byte byteBuffered 		= 	B00000000;
	byte resetInitial		= 	B00000000;
	byte initialBuffer		= 	B00000000;
	unsigned long lastReadMillis = 0;

	byte writeByteBuffered = B00000000;

	volatile byte encoderValues = B00000000;

	uint8_t prevNextCode = 0;
	uint16_t store=0;

	int latency = READ_ELAPSED_TIME;

	byte validCW = B01001011;
	byte validCCW = B11100001;

	uint8_t transmissionStatus = 0;
};

#endif

