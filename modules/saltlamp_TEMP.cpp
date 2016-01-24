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
				
					Serial.print(F("TEMP "));
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
	if (ser_command == F("REG_DHT11")) {

		if (!DEVS.in_use(ser_pin)) {
			DEVS.reg(ser_pin, mTEMP);

			devices[ser_pin].driver = TEMP_DHT11;
			devices[ser_pin].last_read = 0;
			devices[ser_pin].security_interval = 1000;

			response_msg = MSG_OK;
		} else {
			response_msg = MSG_PIN_IN_USE;
		}
		
	} else if (ser_command == F("REG_DALLAS")) {

		if (DEVS.is_device(ser_pin, mOW)) {

			devices[ser_pin].driver = TEMP_DALLAS;
			devices[ser_pin].last_read = 0;
			devices[ser_pin].security_interval = 1000;

			dallas = new DallasTemperature(saltlamp_OW::ow);			
			dallas->begin();
			
			char ad[3];
			for (byte i=0; i < 8; i++) {
				ser_value.substring(2*i, 2*i + 2).toCharArray(ad, 3);
				devices[ser_pin].owaddr[i] = (byte)strtol(ad, NULL, 16);
			}
						
			dallas->setResolution(devices[ser_pin].owaddr, 12);
 
			response_msg = MSG_OK;
		} else {
			response_msg = MSG_NOT_DEVICE;
		}
		
	} else if (ser_command == F("REG_AURIOL433")) {

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

		
	} else if (ser_command == F("READ")) {

		if (DEVS.is_device(ser_pin, mTEMP) 
			|| ( DEVS.is_device(ser_pin, mOW) && devices[ser_pin].driver == TEMP_DALLAS ) 
		) {

			currentMillis = millis();
			if (currentMillis - devices[ser_pin].last_read < devices[ser_pin].security_interval) {
				response_msg = MSG_SECURITY_INTERVAL;
			} else {

				Serial.print(F("TEMP "));
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
								Serial.println(F(" CHECKSUM_ERROR"));
								break;

							case DHTLIB_ERROR_TIMEOUT:
								Serial.println(F(" TIMEOUT"));
								break;

							default:
								Serial.println(F(" ERROR"));
								break;
						}

						break;
					}
					
					case TEMP_DALLAS:
					
						dallas->requestTemperatures();
						Serial.print(" ");
						Serial.print(dallas->getTempC(devices[ser_pin].owaddr));
						Serial.print(" ");
						for (uint8_t i = 0; i < 8; i++) {
							// zero pad the address if necessary
							if (devices[ser_pin].owaddr[i] < 16) Serial.print("0");
							Serial.print(devices[ser_pin].owaddr[i], HEX);
						}
						Serial.println();
						break;
					
					case TEMP_AURIOL433: 

						Serial.println(F(" READ_ON_REQUEST_NOT_SUPPORTED"));
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