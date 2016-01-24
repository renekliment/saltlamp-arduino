#ifndef saltlamp_devices_h
#define saltlamp_devices_h

#include "Arduino.h"
#include "saltlamp_inc_boards.h"

class saltlamp_devices {
	private:
		struct device {
			byte module;
			
			device() : module(0) {}
		};

		device devices[100];
	public:
		void reg(byte pin, byte module);
		bool in_use(byte pin);
		bool is_device(byte pin, byte module);
		bool is_interrupt(byte pin);
		byte get_interrupt(byte pin);
		bool is_pwm(byte pin);
		bool get_pwm_index(byte pin);
};

#endif