#include "saltlamp_inc_messages.h"

void saltlamp_message(HardwareSerial& Serial, byte& response_msg, String& ser_module, String& ser_command, byte& ser_pin, String& ser_string)
{
	if (response_msg > 0) {
		switch (response_msg) {
			case MSG_OK:
				Serial.print(F("OK "));
				Serial.println(ser_string);
				break;
			case MSG_PIN_IN_USE:
				Serial.println(F("ERROR PIN_IN_USE"));
				break;
			case MSG_ONE_PER_MODULE:
				Serial.println(F("ERROR ONE_PER_MODULE"));
				break;
			case MSG_SYNTAX_ERROR:
				Serial.println(F("ERROR SYNTAX_ERROR"));
				break;
			case MSG_CMD_ERROR:
				Serial.println(F("ERROR CMD_ERROR"));
				break;
			case MSG_CMD_NOT_RECO:
				Serial.println(F("ERROR CMD_NOT_RECO"));
				break;
			case MSG_MODULE_NA:
				Serial.println(F("ERROR MODULE_NA"));
				break;
			case MSG_NOT_DEVICE:
				Serial.println(F("ERROR NOT_DEVICE"));
				break;
			case MSG_SECURITY_INTERVAL:
				Serial.println(F("ERROR SECURITY_INTERVAL"));
				break;
			case MSG_NEED_INTERRUPT_PIN:
				Serial.println(F("ERROR NEED_INTERRUPT_PIN"));
				break;
			default:
				break;
		}

		response_msg = 0;
	}
}