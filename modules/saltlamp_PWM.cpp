#include "saltlamp_PWM.h"

void saltlamp_PWM::parse(String &ser_command, byte &ser_pin, String &ser_value)
{
	byte toSet;
	
	if (ser_command == F("REG")) {

		if (!DEVS.in_use(ser_pin)) {
			
			if (!DEVS.is_pwm(ser_pin)) {
				response_msg = MSG_NEED_PWM_PIN;
				return;
			} 
			
			DEVS.reg(ser_pin, mPWM);
			
			byte index = DEVS.get_pwm_index(ser_pin);
			
			devices[index].inverted  = false;
			if (ser_value == F("I")) {
				devices[index].inverted = true;
			}

			pinMode(ser_pin, OUTPUT);

			if (devices[index].inverted) {
				analogWrite(ser_pin, 255);
				devices[index].state  = 255;
			} else {
				analogWrite(ser_pin, 0);
				devices[index].state  = 0;
			}

			response_msg = MSG_OK;
		} else {
			response_msg = MSG_PIN_IN_USE;
		}

	} else if (ser_command == F("WRITE")) {

		if (DEVS.is_device(ser_pin, mPWM)) {
			
			byte index = DEVS.get_pwm_index(ser_pin);
			
			currentMillis = millis();
			if (currentMillis - devices[index].last_read >= devices[index].security_interval) {
				
				devices[index].last_read = currentMillis;
				
				toSet = ser_value.toInt();
				if (devices[index].inverted) {
					toSet = 255 - toSet;
				}
				
				analogWrite(ser_pin, toSet);
				devices[index].state  = toSet;

				response_msg = MSG_OK;
				
			} else {
				response_msg = MSG_SECURITY_INTERVAL;
			}
				
		} else {
			response_msg = MSG_NOT_DEVICE;
		}

	} else if (ser_command == F("GETSTATE")) {

		if (DEVS.is_device(ser_pin, mPWM)) {
			
			byte index = DEVS.get_pwm_index(ser_pin);
			
			Serial.print(F("PWM "));
			Serial.print(index);
			Serial.print(F(" "));
			Serial.println( devices[index].inverted ? 255 -  devices[index].state : devices[index].state );

		} else {
			response_msg = MSG_NOT_DEVICE;
		}
		
	} else if (ser_command == F("SETSECINTERVAL")) {

		if (DEVS.is_device(ser_pin, mPWM)) {

			byte index = DEVS.get_pwm_index(ser_pin);
			devices[index].security_interval = ser_value.toInt();
			
			response_msg = MSG_OK;
		} else {
			response_msg = MSG_NOT_DEVICE;
		}
		
	} else {
		response_msg = MSG_CMD_NOT_RECO;
	}

}
