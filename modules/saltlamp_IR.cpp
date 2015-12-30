// #include "saltlamp_IR.h"
// 
// void saltlamp_IR::setDevice(IRrecv& IR_device_ext)
// {
// 	static device myDevices(IR_device_ext);
// 	devices = &myDevices;
// 	device_set = true;
// }
// 
// void saltlamp_IR::loop()
// {
// 	if (device_set && devices->IR_irrecv.decode(&devices->IR_results)) {
// 		devices->keyCurrent = devices->IR_results.value;
// 
// 		if (devices->IR_results.value != 0) {
// 
// 			/* "Repetition" key? */
// 			if (devices->keyCurrent == 0xFFFFFFFF) {
// 				devices->keyRepeatNum++;
// 
// 				if (devices->keyRepeatNum > 2) {
// 					devices->keyCurrent = devices->keyPrevious;
// 					devices->keyRepeatNum = 0;
// 				}
// 			} else {
// 				devices->keyRepeatNum = 0;
// 			}
// 
// 			if (devices->keyCurrent != 0xFFFFFFFF) {
// 				Serial.print("IR ");
// 				Serial.print(devices->pin);
// 				Serial.print(" ");
// 				Serial.println(devices->keyCurrent, HEX);
// 
// 				devices->keyPrevious = devices->keyCurrent;
// 			}
// 		}
// 
// 		devices->IR_irrecv.resume();
// 	}
// 
// }
// 
// void saltlamp_IR::parse(String &ser_command, byte &ser_pin, String &ser_value)
// {
// 	if (ser_command == F("REG")) {
// 
// 		if (!DEVS.in_use(ser_pin)) {
// 			if (!device_set) {
// 				DEVS.reg(ser_pin, mIR);
// 
// 				static IRrecv IR_irrecv_init(ser_pin);
// 				IR_irrecv_init.enableIRIn();
// 				setDevice(IR_irrecv_init);
// 
// 				devices->pin = ser_pin;
// 
// 				response_msg = MSG_OK;
// 			} else {
// 				response_msg = MSG_ONE_PER_MODULE;
// 			}
// 		} else {
// 			response_msg = MSG_PIN_IN_USE;
// 		}
// 
// 	} else {
// 		response_msg = MSG_CMD_NOT_RECO;
// 	}
// 
// }