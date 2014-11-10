#include "saltlamp_module.h"

class saltlamp_SYS : public saltlamp_module {
	private:
		boolean SYS_configured;

	public:
		void loop() {}
		void parse(String &ser_command, byte &ser_pin, String &ser_value);
		saltlamp_SYS(HardwareSerial& serial_ext, byte& response_msg_ext, saltlamp_devices& DEVS_ext)
			: saltlamp_module(serial_ext, response_msg_ext, DEVS_ext) {
				SYS_configured = false;
			}
};