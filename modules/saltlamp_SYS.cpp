#include "saltlamp_SYS.h"

void saltlamp_SYS::parse(String &ser_command, byte &ser_pin, String &ser_value)
{
	if (ser_command == F("PING")) {

		if (ser_pin >= 80 && ser_pin <= 89) {
			Serial.print(F("SYS_PONG "));
			Serial.println(ser_pin);
		} else {
			response_msg = MSG_SYNTAX_ERROR;
		}

	} else if (ser_command == F("DIAG")) {

		if (ser_pin == 99) {
			//diag();
		} else {
			response_msg = MSG_SYNTAX_ERROR;
		}

	} else if (ser_command == F("MEM")) {

		if (ser_pin == 99) {
			Serial.print("SYS_MEM ");
			Serial.println(freeMemory());
		} else {
			response_msg = MSG_SYNTAX_ERROR;
		}

	} else if (ser_command == F("CONFIG")) {

		if (ser_pin == 99) {
			if (ser_value == "") {
				Serial.print(F("SYS_CONFIG "));
				Serial.println(SYS_configured);
			} else if (ser_value == "1") {
				SYS_configured = true;

				response_msg = MSG_OK;
			} else {
				response_msg = MSG_SYNTAX_ERROR;
			}
		} else {
			response_msg = MSG_SYNTAX_ERROR;
		}

	} else {
		response_msg = MSG_CMD_NOT_RECO;
	}

}