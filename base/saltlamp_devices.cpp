#include "saltlamp_devices.h"

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
	for (byte i=0; i < BOARD_interrupts_COUNT; i++) {
		if (BOARD_interrupts[i] == pin) {
			return true;
		}
	}

	return false;
}

byte saltlamp_devices::get_interrupt(byte pin)
{
	for (byte i=0; i < BOARD_interrupts_COUNT; i++) {
		if (BOARD_interrupts[i] == pin) {
			return i;
		}
	}

	return -1;
}

bool saltlamp_devices::is_pwm(byte pin)
{
	for (byte i=0; i < BOARD_pwms_COUNT; i++) {
		if (BOARD_pwms[i] == pin) {
			return true;
		}
	}

	return false;
}

byte saltlamp_devices::get_pwm_index(byte pin)
{
	for (byte i=0; i < BOARD_pwms_COUNT; i++) {
		if (BOARD_pwms[i] == pin) {
			return i;
		}
	}
}