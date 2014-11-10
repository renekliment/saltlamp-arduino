#include "saltlamp_SYS.h"

void saltlamp_SYS::parse(String &ser_command, byte &ser_pin, String &ser_value)
{
	if (ser_command == "PING") {

		if (ser_pin >= 80 && ser_pin <= 89) {
			Serial.print("SYS_PONG ");
			Serial.println(ser_pin);
		} else {
			response_msg = MSG_SYNTAX_ERROR;
		}

	} else if (ser_command == "DIAG") {

		if (ser_pin == 99) {
			//diag();
		} else {
			response_msg = MSG_SYNTAX_ERROR;
		}

	} else if (ser_command == "MEM") {

		if (ser_pin == 99) {
			Serial.print("SYS_MEM ");
			Serial.println(freeMemory());
		} else {
			response_msg = MSG_SYNTAX_ERROR;
		}

	} else if (ser_command == "CONFIG") {

		if (ser_pin == 99) {
			if (ser_value == "") {
				Serial.print("SYS_CONFIG ");
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