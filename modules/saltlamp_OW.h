#ifndef saltlamp_OW_h
#define saltlamp_OW_h

#include "../base/saltlamp_module.h"
#include <OneWire.h>

class saltlamp_OW : public saltlamp_module {
		
	public:
		static OneWire *ow;
		byte pin;
		
		void parse(String &ser_command, byte &ser_pin, String &ser_value);
		saltlamp_OW(HardwareSerial& serial_ext, byte& response_msg_ext, saltlamp_devices& DEVS_ext)
			: saltlamp_module(serial_ext, response_msg_ext, DEVS_ext) {}
};

#endif