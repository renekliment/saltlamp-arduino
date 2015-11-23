#include "saltlamp_433.h"

void saltlamp_433::loop()
{

}

void saltlamp_433::parse(String &ser_command, byte &ser_pin, String &ser_value)
{
	if (ser_command == F("REG")) {

		if (!DEVS.in_use(ser_pin)) {
			DEVS.reg(ser_pin, m433);
			
			devices[ser_pin].radio = RCSwitch();
			devices[ser_pin].radio.enableTransmit(ser_pin);
			devices[ser_pin].radio.setProtocol(ser_value.toInt());

			response_msg = MSG_OK;
		} else {
			response_msg = MSG_PIN_IN_USE;
		}

	} else if (ser_command == F("SEND")) {

		if (DEVS.is_device(ser_pin, m433)) {

			devices[ser_pin].radio.send(ser_value.toInt(), 24);
	
			response_msg = MSG_OK;
							
		} else {
			response_msg = MSG_NOT_DEVICE;
		}
			
	} else if (ser_command == F("PROTO")) {

		if (DEVS.is_device(ser_pin, m433)) {

			devices[ser_pin].radio.setProtocol(ser_value.toInt());
	
			response_msg = MSG_OK;
							
		} else {
			response_msg = MSG_NOT_DEVICE;
		}
			
	} else {
		response_msg = MSG_CMD_NOT_RECO;
	}

}