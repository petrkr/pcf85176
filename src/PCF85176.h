#ifndef PCF85176_H_
#define PCF85176_H_

#include <Wire.h>

// #define PCF85176_DEBUG

// Define enums for the modes
typedef enum {
    MODE_STATUS_BLANK = 0,
    MODE_STATUS_ENABLED = 1
} ModeStatus;

typedef enum {
    MODE_BIAS_13 = 0,
    MODE_BIAS_12 = 1
} ModeBias;

typedef enum {
    MODE_DRIVE_STATIC = 1,
    MODE_DRIVE_12 = 2,
    MODE_DRIVE_13 = 3,
    MODE_DRIVE_14 = 0
} ModeDrive;

typedef enum {
    BLINK_FREQUENCY_OFF = 0,
    BLINK_FREQUENCY_1 = 1,
    BLINK_FREQUENCY_2 = 2,
    BLINK_FREQUENCY_3 = 3
} BlinkFrequency;

typedef enum {
    BLINK_MODE_NORMAL = 0,
    BLINK_MODE_ALTRAM = 1
} BlinkMode;

// Define commands
#define CMD_LOAD_POINTER	0x00
#define CMD_MODE			0x40
#define CMD_DEVICE_SELECT	0x60
#define CMD_BLINK			0x70
#define CMD_BANK_SELECT		0x78
#define CMD_LAST_COMMAND	0x80

// Max address - TODO: make it somehow display-based
#define MAX_ADDRESS		39
#define BUFFERSIZE		20

class PCF85176 {
	public:
		PCF85176(TwoWire& i2c, uint8_t address = 56, uint8_t subaddress = 0, uint8_t backlight = -1);
		virtual void begin();
		void bankSelect(uint8_t input, uint8_t output);
		void blink(BlinkFrequency blink = BLINK_FREQUENCY_OFF, BlinkMode mode = BLINK_MODE_NORMAL);
		void clear();
		void display();
		void noDisplay();
		void write(uint8_t data, uint8_t address = 0);
		void write(uint8_t *data, size_t length, uint8_t address = 0);

	protected:
		// Private variables
		uint8_t _buffer[BUFFERSIZE];
		TwoWire& _i2c;
		uint8_t _address;
		uint8_t _subaddress;
		uint8_t _backlight;
		ModeBias _bias;
		ModeDrive _drive;

		// Private methods
		void _deviceSelect();
		void _setMode(ModeStatus status, ModeBias bias, ModeDrive drive);

		// Mapping chars to 7-segment bits
		// Every LCD screen class should implement this method
		virtual uint8_t _getCharBits(char character);

};
#endif
