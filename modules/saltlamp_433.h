#include "saltlamp_module.h"

class saltlamp_433 : public saltlamp_module {
	private:
		struct device {
			RCSwitch radio;
		};

		static const byte PINS = BOARD_D_MAX + 1;
		device devices[PINS];

	public:
		void loop();
		void parse(String &ser_command, byte &ser_pin, String &ser_value);
		saltlamp_433(HardwareSerial& serial_ext, byte& response_msg_ext, saltlamp_devices& DEVS_ext)
			: saltlamp_module(serial_ext, response_msg_ext, DEVS_ext) {}
};