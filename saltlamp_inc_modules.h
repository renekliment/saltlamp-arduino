#ifndef saltlamp_inc_modules_h
#define saltlamp_inc_modules_h

enum {
	mDI = 1,
	mDO,
	mAI,
	mTEMP,
	mIR,
	mUS,
	mSYS
} modules;

/* Arduino IDE / compiler / linker / ... is just so efing stupid in so many ways ...
 *
 * you can't include files from subdirectories at the moment
 * however, for some reason, this works (and if we include .cpp files)
 */
#include "external/MemoryFree.cpp"
#include "external/dht.cpp"
#include "external/auriol433.cpp"
#include "external/IRremote/IRremote.cpp"

#include "modules/saltlamp_SYS.cpp"
#include "modules/saltlamp_AI.cpp"
#include "modules/saltlamp_DI.cpp"
#include "modules/saltlamp_DO.cpp"
#include "modules/saltlamp_TEMP.cpp"
#include "modules/saltlamp_IR.cpp"
#include "modules/saltlamp_US.cpp"

#endif