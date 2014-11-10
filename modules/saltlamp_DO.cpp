#include "saltlamp_DO.h"

void saltlamp_DO::parse(String &ser_command, byte &ser_pin, String &ser_value)
{
	if (ser_command == "REG") {

		if (!DEVS.in_use(ser_pin)) {
			DEVS.reg(ser_pin, mDO);
			
			devices[ser_pin].inverted  = false;
			if (ser_value == "I") {
				devices[ser_pin].inverted = true;
			}

			pinMode(ser_pin, OUTPUT);

			if (devices[ser_pin].inverted) {
				digitalWrite(ser_pin, HIGH);
				devices[ser_pin].state  = true;
			} else {
				digitalWrite(ser_pin, LOW);
				devices[ser_pin].state  = false;
			}

			response_msg = MSG_OK;
		} else {
			response_msg = MSG_PIN_IN_USE;
		}

	} else if (ser_command == "WRITE") {

		if (DEVS.is_device(ser_pin, mDO)) {
			
			currentMillis = millis();
			if (currentMillis - devices[ser_pin].last_read >= devices[ser_pin].security_interval) {
				
				devices[ser_pin].last_read = currentMillis;
				
				if (ser_value == "1" || ser_value == "0") {
					/*
								SENDING		INVERTED
								0			0 		=> OFF
								0			1		=> ON
								1			0		=> ON
								1			1		=> OFF
					*/
					if (ser_value.toInt() == devices[ser_pin].inverted) {
						digitalWrite(ser_pin, LOW);
						devices[ser_pin].state  = false;
					} else {
						digitalWrite(ser_pin, HIGH);
						devices[ser_pin].state  = true;
					}

					response_msg = MSG_OK;
				} else {
					response_msg = MSG_SYNTAX_ERROR;
				}
				
			} else {
				response_msg = MSG_SECURITY_INTERVAL;
			}
				
		} else {
			response_msg = MSG_NOT_DEVICE;
		}

	} else if (ser_command == "GETSTATE") {

		if (DEVS.is_device(ser_pin, mDO)) {
			
			Serial.print("DO ");
			Serial.print(ser_pin);
			Serial.print(" ");
			Serial.println( !(devices[ser_pin].state == devices[ser_pin].inverted ) );

		} else {
			response_msg = MSG_NOT_DEVICE;
		}
		
	} else if (ser_command == "SETSECINTERVAL") {

		if (DEVS.is_device(ser_pin, mDO)) {

			devices[ser_pin].security_interval = ser_value.toInt();
			
			response_msg = MSG_OK;
		} else {
			response_msg = MSG_NOT_DEVICE;
		}
		
	} else {
		response_msg = MSG_CMD_NOT_RECO;
	}

}