#include <Arduino.h>
#include "PCF85176.h"


PCF85176::PCF85176(TwoWire& i2c, uint8_t address, uint8_t subaddress) : _i2c(i2c) {
    _address = address;
    _subaddress = subaddress;
}

void PCF85176::begin() {
    _deviceSelect();

    // TODO: This should be probably done in child class
    _setMode(MODE_STATUS_ENABLED, MODE_BIAS_13, MODE_DRIVE_14);
}

void PCF85176::bankSelect(uint8_t input, uint8_t output) {
    uint8_t data;

    data |= CMD_BANK_SELECT;
    data |= input << 1;
    data |= output;
    data |= CMD_LAST_COMMAND;

    _i2c.beginTransmission(_address);
    _i2c.write(data);
    _i2c.endTransmission();
}

void PCF85176::blink(BlinkFrequency frequency, BlinkMode mode) {
    uint8_t data;

    data |= CMD_BLINK;
    data |= mode << 2;
    data |= frequency;
    data |= CMD_LAST_COMMAND;

    _i2c.beginTransmission(_address);
    _i2c.write(data);
    _i2c.endTransmission();
}

void PCF85176::clear() {
    uint8_t tmp[(MAX_ADDRESS / 2) - 1] = { 0 };
    write(tmp, sizeof(tmp), 0);
}

void PCF85176::display() {
    _setMode(MODE_STATUS_ENABLED, _bias, _drive);
}

void PCF85176::noDisplay() {
    _setMode(MODE_STATUS_BLANK, _bias, _drive);
}

void PCF85176::write(uint8_t data, uint8_t address) {
    write(&data, 1, address);
}

void PCF85176::write(uint8_t *data, size_t length, uint8_t address) {
    _i2c.beginTransmission(_address);
    _i2c.write(address);
    _i2c.write(data, length);
    _i2c.endTransmission();
}

void PCF85176::_deviceSelect() {
    uint8_t data;

    data |= CMD_DEVICE_SELECT;
    data |= _subaddress;
    data |= CMD_LAST_COMMAND;

    _i2c.beginTransmission(_address);
    _i2c.write(data);
    _i2c.endTransmission();
}

void PCF85176::_setMode(ModeStatus status, ModeBias bias, ModeDrive drive) {
    _bias = bias;
    _drive = drive;

    uint8_t data;

    data |= CMD_MODE;
    data |= status << 3;
    data |= _bias << 2;
    data |= _drive;
    data |= CMD_LAST_COMMAND;

    _i2c.beginTransmission(_address);
    _i2c.write(data);
    _i2c.endTransmission();
}

uint8_t PCF85176::_getCharBits(char character) {
    switch (character) {
    case '*': // For degree for now
        return 0b0110011;
    case '|':
        return 0b0000101;
    case '-':
        return 0b0000010;
    case '_':
        return 0b0001000;
    case '0':
        return 0b1111101;
    case '1':
        return 0b1100000;
    case '2':
        return 0b111110;
    case '3':
        return 0b1111010;
    case '4':
        return 0b1100011;
    case '5':
        return 0b1011011;
    case '6':
        return 0b1011111;
    case '7':
        return 0b1110000;
    case '8':
        return 0b1111111;
    case '9':
        return 0b1111011;
    case 'A':
    case 'a':
        return 0b1110111;
    case 'b':
    case 'B':
        return 0b1001111;
    case 'c':
    //	return 0b0001110;
    case 'C':
        return 0b0011101;
    case 'd':
    case 'D':
        return 0b1101110;
    case 'e':
    //	return 0b0001110;
    case 'E':
        return 0b0011111;
    case 'f':
    //	return 0b0000111;
    case 'F':
        return 0b0010111;
    case 'G':
    case 'g':
        return 0b1011101;
    case 'h':
    //	return 0b1000111;
    case 'H':
        return 0b1100111;
    case 'i':
    //	return 0b1000000;
    case 'I':
        return 0b1100000;
    case 'J':
    case 'j':
        return 0b1101000;
    case 'l':
    //	return 0b1100000;
    case 'L':
        return 0b0001101;
    case 'm':
    case 'M':
        return 0b1010100;
    case 'n':
    case 'N':
        return 0b1000110;
    case 'O': // we can keep this for zero
    //	return 0b1111101;
    case 'o':
        return 0b1001110;
    case 'P':
    case 'p':
        return 0b0110111;
    case 'q':
    case 'Q':
        return 0b1110011;
    case 'r':
    case 'R':
        return 0b0000110;
    case 'S':
    case 's':
        return 0b1011011;
    case 't':
    case 'T':
        return 0b0001111;
    case 'u':
    //	return 0b1001100;
    case 'U':
        return 0b1101101;
    case 'Y':
    case 'y':
        return 0b1101011;
    case 'z':
    case 'Z':
        return 0b0111110;
    case ' ':
    default:
        return 0b0000000;
    }
}
