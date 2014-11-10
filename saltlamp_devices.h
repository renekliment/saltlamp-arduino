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
};

void saltlamp_devices::reg(byte pin, byte module)
{
	devices[pin].module = module;
}

bool saltlamp_devices::in_use(byte pin)
{
	if ((pin >= BOARD_D_MIN && pin <= BOARD_D_MAX)
		|| (pin >= (BOARD_A_MIN + 60) && pin <= (BOARD_A_MAX + 60))
	) {
		if (devices[pin].module != 0) {
			return true;
		} else {
			return false;
		}
	}

	return true;
}

bool saltlamp_devices::is_device(byte pin, byte module)
{
	if (devices[pin].module == module) {
		return true;
	}

	return false;
}

bool saltlamp_devices::is_interrupt(byte pin)
{
	for (byte i=0; i < (sizeof(BOARD_interrupts)/sizeof(byte)); i++) {
		if (BOARD_interrupts[i] == pin) {
			return true;
		}
	}

	return false;
}

byte saltlamp_devices::get_interrupt(byte pin)
{
	for (byte i=0; i < (sizeof(BOARD_interrupts)/sizeof(byte)); i++) {
		if (BOARD_interrupts[i] == pin) {
			return i;
		}
	}

	return -1;
}

#endif