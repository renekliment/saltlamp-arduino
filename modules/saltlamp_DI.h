#ifndef saltlamp_DI_h
#define saltlamp_DI_h

#include "../base/saltlamp_module.h"

class saltlamp_DI : public saltlamp_module {
	private:
		struct device {
			bool state;
			bool active;
			bool pullup;

			device() : state(false), active(false), pullup(false) {}
		};

		static const byte PINS = BOARD_D_MAX + 1;
		device devices[PINS];

		void send_status(byte pin);
	public:
		void loop();
		void parse(String &ser_command, byte &ser_pin, String &ser_value);
		saltlamp_DI(HardwareSerial& serial_ext, byte& response_msg_ext, saltlamp_devices& DEVS_ext)
			: saltlamp_module(serial_ext, response_msg_ext, DEVS_ext) {}
};

#endif