#include "saltlamp_OW.h"

void saltlamp_OW::parse(String &ser_command, byte &ser_pin, String &ser_value)
{	
	
	if (ser_command == F("REG")) {

		if (!DEVS.in_use(ser_pin)) {
			DEVS.reg(ser_pin, mOW);

			pin = ser_pin;
			ow = new OneWire(pin);
			
			response_msg = MSG_OK;
		} else {
			response_msg = MSG_PIN_IN_USE;
		}
		
	} else {
		response_msg = MSG_CMD_NOT_RECO;
	}

}

OneWire* saltlamp_OW::ow = 0;