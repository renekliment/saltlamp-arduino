#ifndef saltlamp_inc_messages_h
#define saltlamp_inc_messages_h

#include "Arduino.h"

enum {
	MSG_NONE = 0,
	MSG_OK,
	MSG_PIN_IN_USE,
	MSG_ONE_PER_MODULE,
	MSG_SYNTAX_ERROR,
	MSG_CMD_ERROR,
	MSG_CMD_NOT_RECO,
	MSG_MODULE_NA,
	MSG_NOT_DEVICE,
	MSG_SECURITY_INTERVAL,
	MSG_NEED_INTERRUPT_PIN,
	MSG_NEED_PWM_PIN
} messages;

void saltlamp_message(HardwareSerial& Serial, byte& response_msg, String& ser_module, String& ser_command, byte& ser_pin, String& ser_string);

#endif