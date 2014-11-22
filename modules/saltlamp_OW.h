#include "saltlamp_module.h"

class saltlamp_OW : public saltlamp_module {
		
	public:
		static OneWire *ow;
		byte pin;
		
		void parse(String &ser_command, byte &ser_pin, String &ser_value);
		saltlamp_OW(HardwareSerial& serial_ext, byte& response_msg_ext, saltlamp_devices& DEVS_ext)
			: saltlamp_module(serial_ext, response_msg_ext, DEVS_ext) {}
};