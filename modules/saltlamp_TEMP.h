#ifndef saltlamp_TEMP_h
#define saltlamp_TEMP_h

#include "../base/saltlamp_module.h"

#include "../external/Arduino-Temperature-Control-Library/DallasTemperature.h"
#include "../external/dht.h"
#include "../external/auriol433.h"

#include "saltlamp_OW.h"

class saltlamp_TEMP : public saltlamp_module {
	private:
		struct device {
			byte driver;
			unsigned long last_read;
			unsigned int security_interval;
			DeviceAddress owaddr;

			device() : driver(0), last_read(0), security_interval(0) {}
		};

		unsigned long currentMillis;

		static const byte PINS = BOARD_D_MAX + 1;
		device devices[PINS];

		enum {
			TEMP_DHT11 = 1,
			TEMP_AURIOL433,
			TEMP_DALLAS
		} drivers;

		dht DHT;
		auriol433 AURIOL433;
		DallasTemperature *dallas;
		
	public:
		void auriol433check();
		void loop();
		void parse(String &ser_command, byte &ser_pin, String &ser_value);
		saltlamp_TEMP(HardwareSerial& serial_ext, byte& response_msg_ext, saltlamp_devices& DEVS_ext)
			: saltlamp_module(serial_ext, response_msg_ext, DEVS_ext) {}
};

void saltlamp_TEMP_interrupt1();
void saltlamp_TEMP_interrupt2();

#endif