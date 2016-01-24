#include "saltlamp_433.h"

void saltlamp_433::loop()
{
  	for (byte i=0; i<BOARD_interrupts_COUNT; i++) {
		if (devicesR[i].registered && devicesR[i].radio.available()) {

			int value = devicesR[i].radio.getReceivedValue();

			if (value == 0) {
				// Unknown encoding
			} else {
				Serial.print(F("433 "));
				Serial.print( BOARD_interrupts[i] );
				Serial.print(" ");
				Serial.println( devicesR[i].radio.getReceivedValue() );
				
// 				.getReceivedBitlength()
// 				.getReceivedProtocol()
			}

			devicesR[i].radio.resetAvailable();
		}
	}

}

void saltlamp_433::parse(String &ser_command, byte &ser_pin, String &ser_value)
{
	if (ser_command == F("REG_TRANSMITTER")) {

		if (!DEVS.in_use(ser_pin)) {
			DEVS.reg(ser_pin, m433);
			
			devicesT[ser_pin].radio = RCSwitch();
			devicesT[ser_pin].radio.enableTransmit(ser_pin);
			devicesT[ser_pin].radio.setProtocol(ser_value.toInt());

			response_msg = MSG_OK;
		} else {
			response_msg = MSG_PIN_IN_USE;
		}

	} else if (ser_command == F("REG_RECEIVER")) {

		if (!DEVS.in_use(ser_pin)) {
			
			if (!DEVS.is_interrupt(ser_pin)) {
				response_msg = MSG_NEED_INTERRUPT_PIN;
				return;
			} 

			DEVS.reg(ser_pin, m433);
			
			byte interrupt = DEVS.get_interrupt(ser_pin);
			
			devicesR[interrupt].radio = RCSwitch();
			devicesR[interrupt].radio.enableReceive( interrupt );
			devicesR[interrupt].registered = true;

			response_msg = MSG_OK;
		} else {
			response_msg = MSG_PIN_IN_USE;
		}
		
	} else if (ser_command == F("SEND")) {

		if (DEVS.is_device(ser_pin, m433)) {

			devicesT[ser_pin].radio.send(ser_value.toInt(), 24);
	
			response_msg = MSG_OK;
							
		} else {
			response_msg = MSG_NOT_DEVICE;
		}
			
	} else if (ser_command == F("PROTO")) {

		if (DEVS.is_device(ser_pin, m433)) {

			devicesT[ser_pin].radio.setProtocol(ser_value.toInt());
	
			response_msg = MSG_OK;
							
		} else {
			response_msg = MSG_NOT_DEVICE;
		}
			
	} else {
		response_msg = MSG_CMD_NOT_RECO;
	}

}