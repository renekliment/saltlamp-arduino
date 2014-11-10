#ifndef saltlamp_module_h
#define saltlamp_module_h

#include "Arduino.h"
#include "saltlamp_devices.h"

class saltlamp_module {
	protected:
		struct device {
		};

		static const byte PINS = 0;
		device devices[PINS];

		HardwareSerial& Serial;
		byte& response_msg;
		saltlamp_devices& DEVS;
	public:
		virtual void loop() {}
		virtual void parse(String &ser_command, byte &ser_pin, String &ser_value) {}
		saltlamp_module(HardwareSerial& serial_ext, byte& response_msg_ext, saltlamp_devices& DEVS_ext)
			: Serial(serial_ext),response_msg(response_msg_ext),DEVS(DEVS_ext) { }
};

#endif