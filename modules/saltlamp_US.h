#ifndef saltlamp_US_h
#define saltlamp_US_h

#include "../base/saltlamp_module.h"

class saltlamp_US : public saltlamp_module {
	private:
		struct device {
			byte transciever_pin;
			byte echo_pin;

			device() : transciever_pin(0), echo_pin(0) {}
		};

		//static const byte PINS = BOARD_D_MAX + 1;
		//device devices[PINS];

		device devices;

		boolean handWentAway;
		unsigned long handAwayTime;

		byte every;
		unsigned long every_previous;

		volatile float range;
		float rangePrevious;

		volatile unsigned long timer;

	public:
		void measure();
		void loop();
		void parse(String &ser_command, byte &ser_pin, String &ser_value);
		saltlamp_US(HardwareSerial& serial_ext, byte& response_msg_ext, saltlamp_devices& DEVS_ext);
};

void saltlamp_US_interrupt1();
void saltlamp_US_interrupt2();

#endif