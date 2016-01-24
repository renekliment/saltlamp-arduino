#include "saltlamp_AI.h"

void saltlamp_AI::send_status(byte pin)
{
	if (!devices[pin].active) {
		devices[pin].value = analogRead(pin);
	}

	Serial.print(F("AI "));
	Serial.print(pin + 60);
	Serial.print(F(" "));
	Serial.println(devices[pin].value);
}

void saltlamp_AI::loop()
{
	int j;
	int diff;

	for (byte i=0; i<PINS; i++) {
		if (devices[i].active) {
			j = analogRead(i);
			diff = j - devices[i].value;
			
			if ( abs(diff) >= devices[i].diffThreshold ) {
				devices[i].value = j;
				send_status(i);
			}
		}
	}
}

void saltlamp_AI::parse(String &ser_command, byte &ser_pin, String &ser_value)
{
	if (ser_command == F("REG")) {

		if (!DEVS.in_use(ser_pin)) {
			DEVS.reg(ser_pin, mAI);

			ser_pin = ser_pin - 60;
			devices[ser_pin].value = analogRead(ser_pin);

			devices[ser_pin].active = false;
			if (ser_value == F("E")) {
				devices[ser_pin].active = true;
			}

			response_msg = MSG_OK;
		} else {
			response_msg = MSG_PIN_IN_USE;
		}

	} else if (ser_command == F("SET_DIFFTHRSHLD")) {

		if (DEVS.is_device(ser_pin, mAI)) {
			devices[ser_pin - 60].diffThreshold = ser_value.toInt();
			
			response_msg = MSG_OK;
		} else {
			response_msg = MSG_NOT_DEVICE;
		}
		
	} else if (ser_command == F("ENABLE")) {

		if (DEVS.is_device(ser_pin, mAI)) {
			devices[ser_pin - 60].active = true;

			response_msg = MSG_OK;
		} else {
			response_msg = MSG_NOT_DEVICE;
		}

	} else if (ser_command == F("DISABLE")) {

		if (DEVS.is_device(ser_pin, mAI)) {
			devices[ser_pin - 60].active = false;

			response_msg = MSG_OK;
		} else {
			response_msg = MSG_NOT_DEVICE;
		}

	} else if (ser_command == F("READ")) {

		if (DEVS.is_device(ser_pin, mAI)) {
			send_status(ser_pin - 60);
		} else {
			response_msg = MSG_NOT_DEVICE;
		}
	} else {
		response_msg = MSG_CMD_NOT_RECO;
	}

}