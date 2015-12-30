#ifndef saltlamp_IR_h
#define saltlamp_IR_h

// #include "../base/saltlamp_module.h"
// #include "external/IRremote/IRremote.cpp"
// 
// class saltlamp_IR : public saltlamp_module {
// 	private:
// 		struct device {
// 			byte pin;
// 			unsigned long keyPrevious;
// 			unsigned long keyCurrent;
// 			byte keyRepeatNum;
// 
// 			IRrecv &IR_irrecv;
// 			decode_results IR_results;
// 
// 			device(IRrecv& IR_irrecv_ext) : pin(0), keyPrevious(0), keyCurrent(0), keyRepeatNum(0), IR_irrecv(IR_irrecv_ext) {}
// 		};
// 
// 		device *devices;
// 
// 		bool device_set;
// 
// 		void setDevice(IRrecv& IR_device_ext);
// 	public:
// 		void loop();
// 		void parse(String &ser_command, byte &ser_pin, String &ser_value);
// 		saltlamp_IR(HardwareSerial& serial_ext, byte& response_msg_ext, saltlamp_devices& DEVS_ext)
// 			: saltlamp_module(serial_ext, response_msg_ext, DEVS_ext), device_set(false) {}
// };

#endif