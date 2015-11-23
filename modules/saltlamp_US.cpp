#include "saltlamp_US.h"

saltlamp_US::saltlamp_US(HardwareSerial& serial_ext, byte& response_msg_ext, saltlamp_devices& DEVS_ext)
			: saltlamp_module(serial_ext, response_msg_ext, DEVS_ext)
{
	handWentAway = false;
	handAwayTime = 0;

	every = 10;
	every_previous = 0;

	range = 0;
	rangePrevious = 0;

	timer = 0;
}

void saltlamp_US::measure()
{
	if (digitalRead(devices.echo_pin) == HIGH) {
		timer = micros();
	} else {
		if (timer) {
			rangePrevious = range;
			range = (micros() - timer) / 29 / 2;
		}
	}

}

void saltlamp_US::loop()
{
	if (devices.transciever_pin != 0) {

		unsigned long currentMillis = millis();
		if (currentMillis - every_previous >= every) {
			every_previous = currentMillis;

			// Sendout signal
			digitalWrite(devices.transciever_pin, LOW);
			delayMicroseconds(2);
			digitalWrite(devices.transciever_pin, HIGH);
			delayMicroseconds(10);
			digitalWrite(devices.transciever_pin, LOW);

			// Waving detection
			if (range > 5 && range < 15 && handWentAway) {
				handWentAway = false;
				handAwayTime = millis();

				Serial.print("US ");
				Serial.print(devices.transciever_pin);
				Serial.println(" WAVE");
			}

			if ((millis() - handAwayTime) > 1000) {
				handWentAway = true;
			}

		}

	}

}

void saltlamp_US::parse(String &ser_command, byte &ser_pin, String &ser_value)
{
	if (ser_command == F("REG")) {
			if (!DEVS.is_interrupt(ser_value.toInt())) {
				response_msg = MSG_NEED_INTERRUPT_PIN;
			} else {
				if (!DEVS.in_use(ser_pin)) {
					if (!DEVS.in_use(ser_value.toInt())) {
						devices.transciever_pin = ser_pin;
						devices.echo_pin = ser_value.toInt();

						DEVS.reg(devices.transciever_pin, mUS);
						DEVS.reg(devices.echo_pin, mUS);

						pinMode(devices.transciever_pin, OUTPUT);
						pinMode(devices.echo_pin, INPUT);

						response_msg = MSG_OK;
					} else {
						response_msg = MSG_PIN_IN_USE;
					}
				} else {
					response_msg = MSG_PIN_IN_USE;
				}
			}

			attachInterrupt(DEVS.get_interrupt(devices.echo_pin), saltlamp_US_interrupt1, CHANGE);
			
	} else if (ser_command == F("READ")) {

		if (DEVS.is_device(ser_pin, mUS)) {

			Serial.print("US ");
			Serial.print(ser_pin);
			Serial.print(" ");
			Serial.println(range);

		} else {
			response_msg = MSG_NOT_DEVICE;
		}
		
	} else {
		response_msg = MSG_CMD_NOT_RECO;
	}

}