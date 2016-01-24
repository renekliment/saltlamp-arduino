#include "saltlamp_PWM.h"

void saltlamp_PWM::parse(String &ser_command, byte &ser_pin, String &ser_value)
{
	byte toSet;
	
	if (ser_command == F("REG")) {

		if (!DEVS.in_use(ser_pin)) {
			DEVS.reg(ser_pin, mPWM);
			
			devices[ser_pin].inverted  = false;
			if (ser_value == "I") {
				devices[ser_pin].inverted = true;
			}

			pinMode(ser_pin, OUTPUT);

			if (devices[ser_pin].inverted) {
				analogWrite(ser_pin, 255);
				devices[ser_pin].state  = 255;
			} else {
				analogWrite(ser_pin, 0);
				devices[ser_pin].state  = 0;
			}

			response_msg = MSG_OK;
		} else {
			response_msg = MSG_PIN_IN_USE;
		}

	} else if (ser_command == F("WRITE")) {

		if (DEVS.is_device(ser_pin, mPWM)) {
			
			currentMillis = millis();
			if (currentMillis - devices[ser_pin].last_read >= devices[ser_pin].security_interval) {
				
				devices[ser_pin].last_read = currentMillis;
				
				toSet = ser_value.toInt();
				if (devices[ser_pin].inverted) {
					toSet = 255 - toSet;
				}
				
				analogWrite(ser_pin, toSet);
				devices[ser_pin].state  = toSet;

				response_msg = MSG_OK;
				
			} else {
				response_msg = MSG_SECURITY_INTERVAL;
			}
				
		} else {
			response_msg = MSG_NOT_DEVICE;
		}

	} else if (ser_command == F("GETSTATE")) {

		if (DEVS.is_device(ser_pin, mPWM)) {
			
			Serial.print(F("PWM "));
			Serial.print(ser_pin);
			Serial.print(" ");
			Serial.println( devices[ser_pin].inverted ? 255 -  devices[ser_pin].state : devices[ser_pin].state );

		} else {
			response_msg = MSG_NOT_DEVICE;
		}
		
	} else if (ser_command == F("SETSECINTERVAL")) {

		if (DEVS.is_device(ser_pin, mPWM)) {

			devices[ser_pin].security_interval = ser_value.toInt();
			
			response_msg = MSG_OK;
		} else {
			response_msg = MSG_NOT_DEVICE;
		}
		
	} else {
		response_msg = MSG_CMD_NOT_RECO;
	}

}
