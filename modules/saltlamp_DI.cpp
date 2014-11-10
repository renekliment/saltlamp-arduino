#include "saltlamp_DI.h"

void saltlamp_DI::send_status(byte pin)
{
	Serial.print("DI ");
	Serial.print(pin);
	Serial.print(" ");

	if (!devices[pin].active) {
		devices[pin].state = digitalRead(pin);
	}

	/*
	READING		PULLUP		SENDING
	0			0			0
	0			1			1
	1			0			1
	1			1			0
	*/
	Serial.print(!(devices[pin].state == devices[pin].pullup), DEC);
	Serial.println();
}

void saltlamp_DI::loop()
{
	int j;

	for (byte i=0; i<PINS; i++) {
		if (devices[i].active) {
			j = digitalRead(i);
			if (j != devices[i].state) {
				devices[i].state = j;
				send_status(i);
			}
		}
	}
}

void saltlamp_DI::parse(String &ser_command, byte &ser_pin, String &ser_value)
{

	if (ser_command == "REG"
		|| ser_command == "REG_PULLUP"
	) {

		if (!DEVS.in_use(ser_pin)) {
			DEVS.reg(ser_pin, mDI);

			if (ser_command == "REG_PULLUP") {
				pinMode(ser_pin, INPUT_PULLUP);
				devices[ser_pin].pullup = true;
			} else {
				pinMode(ser_pin, INPUT);
				devices[ser_pin].pullup = false;
			}
			devices[ser_pin].state = digitalRead(ser_pin);

			devices[ser_pin].active = true;
			if (ser_value == "D") {
				devices[ser_pin].active = false;
			}

			response_msg = MSG_OK;
		} else {
			response_msg = MSG_PIN_IN_USE;
		}

	} else if (ser_command == "ENABLE") {

		if (DEVS.is_device(ser_pin, mDI)) {
			devices[ser_pin].active = true;

			response_msg = MSG_OK;
		} else {
			response_msg = MSG_NOT_DEVICE;
		}

	} else if (ser_command == "DISABLE") {

		if (DEVS.is_device(ser_pin, mDI)) {
			devices[ser_pin].active = false;

			response_msg = MSG_OK;
		} else {
			response_msg = MSG_NOT_DEVICE;
		}

	} else if (ser_command == "READ") {

		if (DEVS.is_device(ser_pin, mDI)) {
			send_status(ser_pin);
		} else {
			response_msg = MSG_NOT_DEVICE;
		}

	} else {
		response_msg = MSG_CMD_NOT_RECO;
	}

}