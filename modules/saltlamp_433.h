#ifndef saltlamp_433_h
#define saltlamp_433_h

#include "../base/saltlamp_module.h"
#include "../external/rc-switch/RCSwitch.h"

class saltlamp_433 : public saltlamp_module {
	private:
		struct deviceT {
			RCSwitch radio;
		};
		
		struct deviceR {
			RCSwitch radio;
			bool registered;
			
			deviceR() : registered(false) {}
		};

		static const byte PINS = BOARD_D_MAX + 1;
		deviceT devicesT[PINS];
		
		deviceR devicesR[BOARD_interrupts_COUNT];

	public:
		void loop();
		void parse(String &ser_command, byte &ser_pin, String &ser_value);
		saltlamp_433(HardwareSerial& serial_ext, byte& response_msg_ext, saltlamp_devices& DEVS_ext)
			: saltlamp_module(serial_ext, response_msg_ext, DEVS_ext) {}
};

#endif