#include "RtechI2c.h"
#include "Wire.h"


/**
 * Constructor
 * @param address: i2c address
 */
RTECHI2::RTECHI2(uint8_t address){
	_wire = &Wire;

	_address = address;
};

/**
 * Construcor
 * @param address: i2c address
 * @param interruptPin: pin to set interrupt
 * @param interruptFunction: function to call when interrupt raised
 */
RTECHI2::RTECHI2(uint8_t address, uint8_t interruptPin,  void (*interruptFunction)() ){
	_wire = &Wire;

	_address = address;
	_interruptPin = interruptPin;
	_interruptFunction = interruptFunction;
	_usingInterrupt = true;
};

#if !defined(__AVR) && !defined(__STM32F1__) && !defined(TEENSYDUINO)
	/**
	 * Constructor
	 * @param address: i2c address
	 * @param sda: sda pin
	 * @param scl: scl pin
	 */
	RTECHI2::RTECHI2(uint8_t address, uint8_t sda, uint8_t scl){
		_wire = &Wire;

		_address = address;
		_sda = sda;
		_scl = scl;
	};

	/**
	 * Constructor
	 * @param address: i2c address
	 * @param sda: sda pin
	 * @param scl: scl pin
	 * @param interruptPin: pin to set interrupt
 	 * @param interruptFunction: function to call when interrupt raised
	 */
	RTECHI2::RTECHI2(uint8_t address, uint8_t sda, uint8_t scl, uint8_t interruptPin,  void (*interruptFunction)() ){
		_wire = &Wire;

		_address = address;
		_sda = sda;
		_scl = scl;

		_interruptPin = interruptPin;
		_interruptFunction = interruptFunction;

		_usingInterrupt = true;
	};
#endif

#ifdef ESP32
	/**
	 * Constructor
	 * @param address: i2c address
	 */
	RTECHI2::RTECHI2(TwoWire *pWire, uint8_t address){
		_wire = pWire;

		_address = address;
	};

	/**
	 * Construcor
	 * @param address: i2c address
	 * @param interruptPin: pin to set interrupt
	 * @param interruptFunction: function to call when interrupt raised
	 */
	RTECHI2::RTECHI2(TwoWire *pWire, uint8_t address, uint8_t interruptPin,  void (*interruptFunction)() ){
		_wire = pWire;

		_address = address;
		_interruptPin = interruptPin;
		_interruptFunction = interruptFunction;
		_usingInterrupt = true;
	};

	/**
	 * Constructor
	 * @param address: i2c address
	 * @param sda: sda pin
	 * @param scl: scl pin
	 */
	RTECHI2::RTECHI2(TwoWire *pWire, uint8_t address, uint8_t sda, uint8_t scl){
		_wire = pWire;

		_address = address;
		_sda = sda;
		_scl = scl;
	};

	/**
	 * Constructor
	 * @param address: i2c address
	 * @param sda: sda pin
	 * @param scl: scl pin
	 * @param interruptPin: pin to set interrupt
	 * @param interruptFunction: function to call when interrupt raised
	 */
	RTECHI2::RTECHI2(TwoWire *pWire, uint8_t address, uint8_t sda, uint8_t scl, uint8_t interruptPin,  void (*interruptFunction)() ){
		_wire = pWire;

		_address = address;
		_sda = sda;
		_scl = scl;

		_interruptPin = interruptPin;
		_interruptFunction = interruptFunction;

		_usingInterrupt = true;
	};
#endif
	bool encoderPins[8];


bool RTECHI2::begin(){
	this->transmissionStatus = 4;
	#if !defined(__AVR) && !defined(__STM32F1__) && !defined(TEENSYDUINO)
		_wire->begin(_sda, _scl);
	#else
	//			Default pin for AVR some problem on software emulation
	//			#define SCL_PIN _scl
	// 			#define SDA_PIN _sda
		_wire->begin();
	#endif

	// Check if there are pins to set low
	if (writeMode>0 || readMode>0){
		DEBUG_PRINTLN("Set write mode");
		_wire->beginTransmission(_address);


		DEBUG_PRINT("resetInitial pin ");
#ifdef PCF8574_SOFT_INITIALIZATION
		resetInitial = writeModeUp | readModePullUp;
#else
		resetInitial = writeModeUp | readMode;
#endif
		DEBUG_PRINTLN( resetInitial, BIN);

		_wire->beginTransmission(_address);
		_wire->write(resetInitial);

		initialBuffer = writeModeUp | readModePullUp;
		byteBuffered = initialBuffer;
		writeByteBuffered = writeModeUp;

		DEBUG_PRINTLN("Start end trasmission if stop here check pullup resistor.");
		this->transmissionStatus = _wire->endTransmission();
	}


	// inizialize last read
	lastReadMillis = millis();

	return this->isLastTransmissionSuccess();
}

/**
 * Set if fin is OUTPUT or INPUT
 * @param pin: pin to set
 * @param mode: mode, supported only INPUT or OUTPUT (to simplify)
 * @param output_start: output_start, for OUTPUT we can set initial value
 */
void RTECHI2::pinMode(uint8_t pin, uint8_t mode, uint8_t output_start){
	DEBUG_PRINT("Set pin ");
	DEBUG_PRINT(pin);
	DEBUG_PRINT(" as ");
	DEBUG_PRINTLN(mode);

	if (mode == OUTPUT){
		writeMode = writeMode | bit(pin);
		if (output_start==HIGH) {
			writeModeUp = writeModeUp | bit(pin);
		}

		readMode =  readMode & ~bit(pin);
		readModePullDown 	=	readModePullDown 	& 	~bit(pin);
		readModePullUp 		=	readModePullUp 		& 	~bit(pin);

		DEBUG_PRINT("W: ");
		DEBUG_PRINT(writeMode, BIN);
		DEBUG_PRINT(" R ALL: ");
		DEBUG_PRINT(readMode, BIN);

		DEBUG_PRINT(" R Down: ");
		DEBUG_PRINT(readModePullDown, BIN);
		DEBUG_PRINT("R Up: ");
		DEBUG_PRINTLN(readModePullUp, BIN);

	}else if (mode == INPUT){
		writeMode = writeMode & ~bit(pin);

		readMode 			=   readMode 			| bit(pin);
		readModePullDown 	=	readModePullDown 	| bit(pin);
		readModePullUp 		=	readModePullUp 		& ~bit(pin);

		DEBUG_PRINT("W: ");
		DEBUG_PRINT(writeMode, BIN);
		DEBUG_PRINT(" R ALL: ");
		DEBUG_PRINT(readMode, BIN);

		DEBUG_PRINT(" R Down: ");
		DEBUG_PRINT(readModePullDown, BIN);
		DEBUG_PRINT("R Up: ");
		DEBUG_PRINTLN(readModePullUp, BIN);
	}else if (mode == INPUT_PULLUP){
		writeMode = writeMode & ~bit(pin);

		readMode 			=   readMode 			| bit(pin);
		readModePullDown 	=	readModePullDown 	& ~bit(pin);
		readModePullUp 		=	readModePullUp 		| bit(pin);

		DEBUG_PRINT("W: ");
		DEBUG_PRINT(writeMode, BIN);
		DEBUG_PRINT(" R ALL: ");
		DEBUG_PRINT(readMode, BIN);

		DEBUG_PRINT(" R Down: ");
		DEBUG_PRINT(readModePullDown, BIN);
		DEBUG_PRINT("R Up: ");
		DEBUG_PRINTLN(readModePullUp, BIN);
	}
	else{
		DEBUG_PRINTLN("Mode non supported by PCF8574")
	}
};


byte getBit(byte n, byte position)
{
   return (n >> position) & 1;
}



/**
 * Write on pin
 * @param pin
 * @param value
 */
bool RTECHI2::digitalWrite(uint8_t pin, uint8_t value){
	DEBUG_PRINTLN("Begin trasmission");
	_wire->beginTransmission(_address);    
	DEBUG_PRINT("Value ");
	DEBUG_PRINT(value);
	DEBUG_PRINT(" Write data pre ");
	DEBUG_PRINT(writeByteBuffered, BIN);

	if (value==HIGH){
		writeByteBuffered = writeByteBuffered | bit(pin);
		byteBuffered  = writeByteBuffered | bit(pin);
	}else{
		writeByteBuffered = writeByteBuffered & ~bit(pin);
		byteBuffered  = writeByteBuffered & ~bit(pin);
	}
	DEBUG_PRINT("Write data ");
	DEBUG_PRINT(writeByteBuffered, BIN);
	DEBUG_PRINT(" for pin ");
	DEBUG_PRINT(pin);
	DEBUG_PRINT(" bin value ");
	DEBUG_PRINT(bit(pin), BIN);
	DEBUG_PRINT(" value ");
	DEBUG_PRINT(value);

	// writeByteBuffered = writeByteBuffered & (~writeMode & byteBuffered);
	byteBuffered = (writeByteBuffered & writeMode) | (resetInitial & readMode);

	// byteBuffered = (writeByteBuffered & writeMode) | (byteBuffered & readMode);
	DEBUG_PRINT(" byteBuffered ");
	DEBUG_PRINTLN(byteBuffered, BIN);

	DEBUG_PRINT("Going to write data ");
	DEBUG_PRINTLN(writeByteBuffered, BIN);

	_wire->write(byteBuffered);

	byteBuffered = (writeByteBuffered & writeMode) | (initialBuffer & readMode);

//	byteBuffered = (writeByteBuffered & writeMode) & (byteBuffered & readMode);
	DEBUG_PRINTLN("Start end trasmission if stop here check pullup resistor.");

	this->transmissionStatus = _wire->endTransmission();

	return this->isLastTransmissionSuccess();
};
