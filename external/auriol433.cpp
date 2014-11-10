#include "auriol433.h"

// Interrupt service routine
void auriol433::PinChangeISR0() {                           
	// Get current time
	unsigned long Time = micros();                          
	
	if (digitalRead(pin) == LOW) {
	// Falling edge
		if (Time > (rise_Time + glitch_Length)) {
		// Not a glitch
			
			// Subtract last falling edge to get pulse time.
			Time = micros() - fall_Time;                        
			if (bitRead(build_Buffer[1],31) == 1)
				bitSet(isrFlags, F_CARRY_BIT);
			else
				bitClear(isrFlags, F_CARRY_BIT);

			if (bitRead(isrFlags, F_STATE) == 1) {
			// Looking for Data
				if ((Time > bit0_MIN) && (Time < bit0_MAX)) {
				// 0 bit
					build_Buffer[1] = build_Buffer[1] << 1;
					build_Buffer[0] = build_Buffer[0] << 1;
					
					if (bitRead(isrFlags,F_CARRY_BIT) == 1)
					bitSet(build_Buffer[0],0);
					bit_Count++;
				} else if ((Time > bit1_MIN) && (Time < bit1_MAX)) {
				// 1 bit
					build_Buffer[1] = build_Buffer[1] << 1;
					bitSet(build_Buffer[1],0);
					build_Buffer[0] = build_Buffer[0] << 1;
					
					if (bitRead(isrFlags,F_CARRY_BIT) == 1)
					bitSet(build_Buffer[0],0);
					bit_Count++;
				} else {
				// Not a 0 or 1 bit so restart data build and check if it's a sync?
					bit_Count = 0;
					build_Buffer[0] = 0;
					build_Buffer[1] = 0;
					bitClear(isrFlags, F_GOOD_DATA);                // Signal data reads dont' match
					bitClear(isrFlags, F_STATE);                    // Set looking for Sync mode
					
					if ((Time > sync_MIN) && (Time < sync_MAX)) {
					// Sync length okay
						bitSet(isrFlags, F_STATE);                    // Set data mode
					}
				}
				
				if (bit_Count >= allDataBits) {
				// All bits arrived
					bitClear(isrFlags, F_GOOD_DATA);                // Assume data reads don't match
					if (build_Buffer[0] == read_Buffer[0]) {
						if (build_Buffer[1] == read_Buffer[1]) 
							bitSet(isrFlags, F_GOOD_DATA);              // Set data reads match
					}
					read_Buffer[0] = build_Buffer[0];
					read_Buffer[1] = build_Buffer[1];
					bitSet(isrFlags, F_HAVE_DATA);                  // Set data available
					bitClear(isrFlags, F_STATE);                    // Set looking for Sync mode
					//digitalWrite(13,HIGH); // Used for debugging
					build_Buffer[0] = 0;
					build_Buffer[1] = 0;
					bit_Count = 0;
				}
			} else {
			// Looking for sync
				if ((Time > sync_MIN) && (Time < sync_MAX)) {
				// Sync length okay
					build_Buffer[0] = 0;
					build_Buffer[1] = 0;
					bit_Count = 0;
					bitSet(isrFlags, F_STATE);                      // Set data mode
					
					//digitalWrite(13, LOW); // Used for debugging
				}
			}
			
			fall_Time = micros();                               // Store fall time
		}
	} else {
	// Rising edge
		if (Time > (fall_Time + glitch_Length)) {
			// Not a glitch
			rise_Time = Time;                                   // Store rise time
		}
	}
	
}

auriol433::reading auriol433::loop() {
	unsigned long myData0 = 0;
	unsigned long myData1 = 0;

	if (bitRead(isrFlags,F_GOOD_DATA) == 1) {
		
		reading ret;
		ret.ok = true;
		
		// We have at least 2 consecutive matching reads
		myData0 = read_Buffer[0]; // Read the data spread over 2x 32 variables
		myData1 = read_Buffer[1];
		bitClear(isrFlags,F_HAVE_DATA); // Flag we have read the data
		//dec2binLong(myData0,DataBits0);
		//dec2binLong(myData1,DataBits1);

		//Serial.print(" - Battery=");
		byte H = (myData1 >> 26) & 0x3;   // Get Battery
		//Serial.print(H);
		
		//Serial.print(" Channel=");
		H = ((myData1 >> 24) & 0x3) + 1;        // Get Channel
		//Serial.print(H);

		//Serial.print(" Temperature=");
		byte ML = (myData1 >> 12) & 0xF0; // Get MMMM
		//     Serial.print(" (M=");
		//     Serial.print(ML);
		H = (myData1 >> 12) & 0xF;        // Get LLLL
		//     Serial.print(" L=");
		//     Serial.print(H);
		ML = ML | H;                      // OR MMMM & LLLL nibbles together
		H = (myData1 >> 20) & 0xF;        // Get HHHH
		//     Serial.print(" H=");
		//     Serial.print(H);
		//     Serial.print(" T= ");
		byte HH = 0;
		if((myData1 >> 23) & 0x1 == 1) //23 bit
				HH = 0xF;
		int Temperature = (H << 8)|(HH << 12) | ML;  // Combine HHHH MMMMLLLL
		//     Serial.print( Temperature);
		//     Serial.print(") ");
		// Temperature = Temperature*3; //F // Remove Constant offset
		//Serial.print(Temperature/10.0,1);   
		
		ret.temperature = Temperature / 10.0;
		
		//Serial.print("C Humidity=");
		H = (myData1 >> 0) & 0xF0;        // Get HHHH
		//     Serial.print(" (H=");
		//     Serial.print(H);
		ML = (myData1 >> 0) & 0xF;       // Get LLLL
		//     Serial.print(" L=");
		//     Serial.print(ML);
		//     Serial.print(") ");
		ML = ML | H;                      // OR HHHH & LLLL nibbles together
		
		ret.humidity = ML;
		//Serial.print(ML);
		//Serial.println("%");
		
		return ret;
	} else {
		return auriol433::nullReading;
	}
}
