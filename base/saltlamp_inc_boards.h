#ifndef saltlamp_inc_boards_h
#define saltlamp_inc_boards_h

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
	//								  0  1   2   3   4   5
	const byte BOARD_interrupts[6] = {2, 3, 21, 20, 19, 18};

	const byte BOARD_serials[] = {0, 1, 19, 18, 17, 16, 15, 14};

	const byte BOARD_pwms_COUNT = 15;
	const byte BOARD_pwms[BOARD_pwms_COUNT] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 44, 45, 46};
	
	const byte BOARD_D_MIN = 0;
	const byte BOARD_D_MAX = 53;
	const byte BOARD_A_MIN = 0;
	const byte BOARD_A_MAX = 15;

#else
	//								  0  1
	const byte BOARD_interrupts[2] = {2, 3};

	const byte BOARD_serials[] = {0, 1};

	const byte BOARD_pwms_COUNT = 6;
	const byte BOARD_pwms[BOARD_pwms_COUNT] = {3, 5, 6, 9, 10, 11};
	
	const byte BOARD_D_MIN = 0;
	const byte BOARD_D_MAX = 13;
	const byte BOARD_A_MIN = 0;
	const byte BOARD_A_MAX = 5;

#endif

#endif