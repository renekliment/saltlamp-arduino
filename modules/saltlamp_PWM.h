#ifndef saltlamp_PWM_h
#define saltlamp_PWM_h

#include "../base/saltlamp_module.h"

class saltlamp_PWM : public saltlamp_module {
	private:
		struct device {
			bool inverted;
			byte state;
			
			unsigned long last_read;
			unsigned int security_interval;

			device() : last_read(0), security_interval(0) {}
		};
		
		unsigned long currentMillis;

		device devices[BOARD_pwms_COUNT];

	public:
		void loop() {}
		void parse(String &ser_command, byte &ser_pin, String &ser_value);
		saltlamp_PWM(HardwareSerial& serial_ext, byte& response_msg_ext, saltlamp_devices& DEVS_ext)
			: saltlamp_module(serial_ext, response_msg_ext, DEVS_ext) {}
};

#endif