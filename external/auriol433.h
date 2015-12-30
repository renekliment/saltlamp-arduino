// Unknown 433Mhz weather sensor decoder. Untested in the real world.
// http://arduino.cc/forum/index.php/topic,142871.msg1106336.html#msg1106336

// __           ___       ___    ___
//   |         |  |      |  |   |  |
//   |_________|  |______|  |___|  |
//
//   |  Sync      |    1    |  0   |
//   |  8320us    | 4500us  | 2530us

// Defines
#define DataBits0 4                                       // Number of data0 bits to expect
#define DataBits1 32                                      // Number of data1 bits to expect
#define allDataBits 36                                    // Number of data sum 0+1 bits to expect

// isrFlags bit numbers
#define F_HAVE_DATA 1                                     // 0=Nothing in read buffer, 1=Data in read buffer
#define F_GOOD_DATA 2                                     // 0=Unverified data, 1=Verified (2 consecutive matching reads)
#define F_CARRY_BIT 3                                     // Bit used to carry over bit shift from one long to the other
#define F_STATE 7                                         // 0=Sync mode, 1=Data mode

#include "Arduino.h"

class auriol433 {
	private:
		
		byte pin;
				
		// Interrupt variables
		unsigned long fall_Time;                              // Placeholder for microsecond time when last falling edge occured.
		unsigned long rise_Time;                              // Placeholder for microsecond time when last rising edge occured.
		byte bit_Count;                                       // Bit counter for received bits.
		unsigned long build_Buffer[2];                     // Placeholder last data packet being received.
		volatile unsigned long read_Buffer[2];             // Placeholder last full data packet read.
		volatile byte isrFlags;                               // Various flag bits

		// Constants
		static const unsigned long sync_MIN = 4300;                      // Minimum Sync time in micro seconds
		static const unsigned long sync_MAX = 4700;

		static const unsigned long bit1_MIN = 2300;
		static const unsigned long bit1_MAX = 2700;

		static const unsigned long bit0_MIN = 1330;
		static const unsigned long bit0_MAX = 1730;

		static const unsigned long glitch_Length = 300;                  // Anything below this value is a glitch and will be ignored.

		
	public:
		
		struct reading {
			float temperature;
			byte humidity;
			bool ok;
		};
		
		reading nullReading;
		
		void setPin(byte p) { pin = p; }
		void PinChangeISR0();
		auriol433::reading loop();
		
		auriol433() 
		{
			nullReading.ok = false;
			
			// Interrupt variables
			fall_Time = 0;                              // Placeholder for microsecond time when last falling edge occured.
			rise_Time = 0;                              // Placeholder for microsecond time when last rising edge occured.
			bit_Count = 0;                                       // Bit counter for received bits.
			build_Buffer[0] = 0;                     // Placeholder last data packet being received.
			build_Buffer[1] = 0;                     // Placeholder last data packet being received.
			read_Buffer[0] = 0;             // Placeholder last full data packet read.
			read_Buffer[1] = 0;             // Placeholder last full data packet read.
			isrFlags = 0;                               // Various flag bits
		}
};