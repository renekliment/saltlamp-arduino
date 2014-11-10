#include "saltlamp_TEMP.h"

void saltlamp_TEMP::auriol433check()
{
	AURIOL433.PinChangeISR0();
}

void saltlamp_TEMP::loop()
{
	
	for (byte i=0; i<PINS; i++) {
		if (devices[i].driver == TEMP_AURIOL433) {

			currentMillis = millis();
			if (currentMillis - devices[i].last_read >= devices[i].security_interval) {
				devices[i].last_read = currentMillis;
				
				auriol433::reading ret;
				ret = AURIOL433.loop();
				
				if (ret.ok) {
				
					Serial.print("TEMP_READ ");
					Serial.print(i);
					Serial.print(" ");
					Serial.print(ret.temperature, 1);
					Serial.print(" ");
					Serial.println(ret.humidity);
				}	
			}
			
		}
	}
	
}

void saltlamp_TEMP::parse(String &ser_command, byte &ser_pin, String &ser_value)
{
	if (ser_command == "REG_DHT11") {

		if (!DEVS.in_use(ser_pin)) {
			DEVS.reg(ser_pin, mTEMP);

			devices[ser_pin].driver = TEMP_DHT11;
			devices[ser_pin].last_read = 0;
			devices[ser_pin].security_interval = 1000;

			response_msg = MSG_OK;
		} else {
			response_msg = MSG_PIN_IN_USE;
		}

	} else if (ser_command == "REG_AURIOL433") {

		if (!DEVS.in_use(ser_pin)) {
			if (!DEVS.is_interrupt(ser_pin)) {
				response_msg = MSG_NEED_INTERRUPT_PIN;
			} else {
				DEVS.reg(ser_pin, mTEMP);
				
				AURIOL433.setPin(ser_pin);
				
				attachInterrupt(DEVS.get_interrupt(ser_pin), saltlamp_TEMP_interrupt1, CHANGE);

				devices[ser_pin].driver = TEMP_AURIOL433;
				devices[ser_pin].last_read = 0;
				devices[ser_pin].security_interval = 1000;

				response_msg = MSG_OK;
			}
		} else {
			response_msg = MSG_PIN_IN_USE;
		}

		
	} else if (ser_command == "READ") {

		if (DEVS.is_device(ser_pin, mTEMP)) {

			currentMillis = millis();
			if (currentMillis - devices[ser_pin].last_read < devices[ser_pin].security_interval) {
				response_msg = MSG_SECURITY_INTERVAL;
			} else {

				Serial.print("TEMP_READ ");
				Serial.print(ser_pin);
				
				devices[ser_pin].last_read = currentMillis;
				
				switch (devices[ser_pin].driver) {
					case TEMP_DHT11: {

						int chk = DHT.read11(ser_pin);
						switch (chk) {
							case DHTLIB_OK:
								Serial.print(" ");
								Serial.print((float)DHT.temperature, 2);
								Serial.print(" ");
								Serial.print((float)DHT.humidity, 2);
								Serial.println();
								break;

							case DHTLIB_ERROR_CHECKSUM:
								Serial.println(" CHECKSUM_ERROR");
								break;

							case DHTLIB_ERROR_TIMEOUT:
								Serial.println(" TIMEOUT");
								break;

							default:
								Serial.println(" ERROR");
								break;
						}

						break;
					}
					
					case TEMP_AURIOL433: 

						Serial.println(" READ_ON_REQUEST_NOT_SUPPORTED");
						break;
						
					default:
						break;
				}

			}
		} else {
			response_msg = MSG_NOT_DEVICE;
		}

	} else {
		response_msg = MSG_CMD_NOT_RECO;
	}

}