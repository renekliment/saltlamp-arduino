#include "Arduino.h"
#include <OneWire.h>

#include "saltlamp_inc_boards.h"
#include "saltlamp_inc_messages.h"
#include "saltlamp_devices.h"
#include "saltlamp_inc_modules.h"

// Variables for parsing PC commands
String ser_string = "";
String ser_module = "";
String ser_command = "";
byte   ser_pin = 0;
String ser_value = "";
boolean ser_string_complete = false;

saltlamp_devices DEVS;

void setup()
{
	Serial.begin(19200);

	ser_string.reserve(42);
	ser_module.reserve(5);
	ser_command.reserve(15);
	ser_value.reserve(17);

	DEVS.reg(0, mSYS); 	// PINs 0 and 1 are reserved for serial communication
	DEVS.reg(1, mSYS);
}

saltlamp_OW OW(Serial, response_msg, DEVS);
saltlamp_SYS SYS(Serial, response_msg, DEVS);
saltlamp_AI AI(Serial, response_msg, DEVS);
saltlamp_DI DI(Serial, response_msg, DEVS);
saltlamp_DO DO(Serial, response_msg, DEVS);
saltlamp_TEMP TEMP(Serial, response_msg, DEVS);
void saltlamp_TEMP_interrupt1() { TEMP.auriol433check(); }

saltlamp_IR IR(Serial, response_msg, DEVS);

saltlamp_US US(Serial, response_msg, DEVS);
void saltlamp_US_interrupt1() { US.measure(); }

void loop()
{
	DI.loop();
	AI.loop();
	US.loop();
	TEMP.loop();
	IR.loop();

	if (ser_string_complete) {
		ser_module = ser_string.substring(0, ser_string.indexOf('_'));
		ser_command = ser_string.substring(ser_string.indexOf('_') + 1, ser_string.indexOf(' '));
		ser_pin = byte(ser_string.substring(ser_string.indexOf(' ') + 1, ser_string.indexOf(' ') + 3).toInt());
		ser_value = ser_string.substring(ser_string.indexOf(' ') + 1);
		if (ser_value.indexOf(' ') == -1) {
			ser_value = "";
		} else {
			ser_value = ser_value.substring(ser_value.indexOf(' ') + 1);
			ser_value.trim();
		}

		if (ser_module == "AI") {
			AI.parse(ser_command, ser_pin, ser_value);
		} else if (ser_module == "DI") {
			DI.parse(ser_command, ser_pin, ser_value);
		} else if (ser_module == "DO") {
			DO.parse(ser_command, ser_pin, ser_value);
		} else if (ser_module == "OW") {
			OW.parse(ser_command, ser_pin, ser_value);
		} else if (ser_module == "TEMP") {
			TEMP.parse(ser_command, ser_pin, ser_value);
		} else if (ser_module == "US") {
			US.parse(ser_command, ser_pin, ser_value);
		} else if (ser_module == "SYS") {
			SYS.parse(ser_command, ser_pin, ser_value);
		} else if (ser_module == "IR") {
			IR.parse(ser_command, ser_pin, ser_value);
		} else {
			response_msg = MSG_MODULE_NA;
		}

		saltlamp_message(Serial, response_msg, ser_module, ser_command, ser_pin, ser_string);

		ser_string = "";
		ser_module = "";
		ser_command = "";
		ser_pin = 0;
		ser_string_complete = false;
	}
}

void serialEvent() {
	while (Serial.available()) {
		char inChar = (char)Serial.read();

		if (inChar == '\n') {
			ser_string_complete = true;
		} else {
			ser_string += inChar;
		}
	}
}