#include "saltlamp_module.h"

class saltlamp_AI : public saltlamp_module {
	private:
		struct device {
			int value;
			bool active;

			device() : value(0), active(false) {}
		};

		static const byte PINS = BOARD_A_MAX + 60 + 1;
		device devices[PINS];

		void send_status(byte pin);
	public:
		void loop();
		void parse(String &ser_command, byte &ser_pin, String &ser_value);
		saltlamp_AI(HardwareSerial& serial_ext, byte& response_msg_ext, saltlamp_devices& DEVS_ext)
			: saltlamp_module(serial_ext, response_msg_ext, DEVS_ext) {}
};