# Legend

|	|                                                        |
|-------|--------------------------------------------------------|
| \#N	| device pin number                                      |
| <-	| prefixes lines that the Arduino itself sends to the PC |

# Syntax
The general syntax is:

`moduleName_command pinNumber optionalParameterForSomeCommands`

so for example: 

`DO_WRITE 13 1`

means:

module:		DO
command:	WRITE
pin:		13
value:		1

as mentioned, the parameter (also called a value) is optional for some commands,
for example, this command does not require it:

`DI_REG 12`

(registers an DI device on pin 12)

however you could pass it a parameter "D" like this:

`DI_REG 12 D`

(you can check the module documentation for what this means)

# Pin mapping (virtual pins <--> real pins)

MEGA	| UNO     | DESCRIPTION
--------|---------|-------------
00 - 53 | 00 - 13 | DIGITAL
54 - 59 | 14 - 59 | ---
60 - 75	| 60 - 65 | ANALOG
76 - 79	| 66 - 79 | ---
80 - 89	| 80 - 89 | PING - PONG
90 - 99	| 90 - 99 | SYSTEM PINs

# Return / error states
With every command you get a state back. You should check for this.

If your command CMD was completed successfully, you get an "OK CMD" message back.
Example:
```
-> DO_REG 13
<- OK DO_REG 13
-> DO_WRITE 13 1
<- OK DO_WRITE 13 1
```
If there is an error, you get a message prefixed with "ERROR ".
Common error messages are these:

Message				| Meaning
--------------------------------|---------
"ERROR PIN_IN_USE"		| The pin is in use already.
"ERROR ONE_PER_MODULE"		| This module can handle one device only.
"ERROR SYNTAX_ERROR"		| Syntax error.
"ERROR CMD_ERROR"		| Command error. Maybe you are doing something wrong? Check your command.
"ERROR CMD_NOT_RECO"		| Command not recognized. For example you are sending "DO_RED 13" instead of "DO_REG 13"
"ERROR MODULE_NA"		| Requested module not available.
"ERROR NOT_DEVICE"		| Device does not exist or is not of the kind you think it is. For example you are trying to do a "DO_WRITE" on an AI device
"ERROR SECURITY_INTERVAL"	| You are trying to read or write (to) the device before the security interval has passed.
"ERROR NEED_INTERRUPT_PIN"	| You are probably trying to register a device on a pin that does not have an interrupt, but the device needs it (for example an ultrasonic sensor or a wireless receiver).
"ERROR NEED_PWM_PIN"		| You are probably trying to register a device on a pin that does not have PWM.

# Modules

## DI (digital input)
```
// registers a device, 
// automatically enables sending messages on device state change
DI_REG #N 

// registers a device, 
// disables sending messages on device state change
DI_REG #N D

// registers a device and enables internal pullup resistor, 
// automatically enables sending messages on device state change
DI_REG_PULLUP #N

// registers a device and enables internal pullup resistor, 
// disables sending messages on device state change
DI_REG_PULLUP #N D

// enables sending messages on device state change (on-the-fly setting)
DI_ENABLE #N

// disables sending messages on device state change (on-the-fly setting)
DI_DISABLE #N

// requests a message with current device state
DI_READ #N

// current device state (sent on request or automatically depending on the setting)
// VALUE is either 1 or 0
<- DI #N VALUE
```
	
## DO (digital output)
```
// registers a device
DO_REG #N

// registers a device, that has inverted polarity,
// meaning that if you set the pin on HIGH, it turns off and vice versa,
// this exists here to provide unified interface for further communication,
// meaning you only need to mention the fact here and then you can work with it as with normal device
DO_REG #N I

// set security interval for a device,
// where VALUE is a number in miliseconds
// meaning you can change the state of a device only once in VALUE milis,
// useful for example for relays
// (you don't have to cover this in you app (timers, etc), you can just set it here instead)
DO_SETSECINTERVAL #N VALUE

// set device state
// VALUE can be either 0 (turn off), or 1 (turn on)
DO_WRITE #N VALUE

// requests device state
DO_GETSTATE #N

// sends device state on request
// VALUE can be either 0 (turned off), or 1 (turned on)
<- DO #N VALUE
```

## AI (analog input)
```
// registers a device, 
// automatically disables sending messages on device state change
AI_REG #N

// registers a device, 
// enables sending messages on device state change
AI_REG #N E

// sets a threshold for disinguishing AI values
// this is set to 1 by default, meaning any change in reading is considered a change
// when you set this to K, then only reading with difference K or more is considered a change
//
// VALUE is a number between 1 and like 1023
// for sensors like photoresistors, 10 or 15 seems like a value you would probably use
AI_SET_DIFFTHRSHLD #N VALUE

// enables sending messages on device state change (on-the-fly setting)
AI_ENABLE #N

// disables sending messages on device state change (on-the-fly setting)
AI_DISABLE #N

// requests a message with current device state
AI_READ #N

// current device state (sent on request or automatically depending on the setting)
// VALUE is from 0 to 1023
<- AI #N VALUE
```

## PWM (PWM output)
```
// registers a device
PWM_REG #N

// registers a device, that has "inverted values",
// meaning that if you set the pin to X, it will actually be set to 255 - X,
// this exists here to provide unified interface for further communication,
// meaning you only need to mention the fact here and then you can work with it as with normal device
PWM_REG #N I

// set security interval for a device,
// where VALUE is a number in miliseconds
// meaning you can change the state of a device only once in VALUE milis,
// (you don't have to cover this in you app (timers, etc), you can just set it here instead)
PWM_SETSECINTERVAL #N VALUE

// set device state
// where 0 <=VALUE <= 255
PWM_WRITE #N VALUE

// requests device state
PWM_GETSTATE #N

// sends device state on request
// where 0 <=VALUE <= 255
<- PWM #N VALUE
```

## 433 (communication with devices over protocols that use 433MHz)
```
// registers a transmitter device
// where PROTOCOL is int
// for protocol number, see the rc-switch library
433_REG_TRANSMITTER #N PROTOCOL

// sends out a code
// CODE is a decimal number, representing a 24-bit long code
433_SEND #N CODE

// switches protocol to PROTOCOL
433_PROTO #N PROTOCOL

// registers a receiver device
// the #N has to be a pin with an interrupt
433_REG_RECEIVER #N

// decoded key code by the receiver
<- 433 #N CODE
```

## OW (1-Wire)
```
// registers 1-Wire bus
OW_REG #N
```

## TEMP (temperature and humidity readings)
```
// registers a DHT11 device
TEMP_REG_DHT11 #N

// registers a "Dallas device", ex. DS18B20
// ADDRESS is 16 characters representing 1-Wire address
TEMP_REG_DALLAS #N ADDRESS

// registers a wireless sensor communicating on 433MHz over the Auriol protocol
TEMP_REG_AURIOL433 #N

// requests a read of a device 
TEMP_READ #N

// message with a reading
// VALUE1 is usually a float meaning temperature
// VALUE2 is usually a float meaning humidity or it can be sensor ID
<- TEMP #N VALUE1 VALUE2
```

## IR (infrared signals)
```
// registers a device
IR_REG #N

// a message with an IR code
<- IR #N VALUE
```

## SYS (system module)
```
// should return some diagnostics ... not implemented yet
SYS_DIAG 99

// simple test of communication; #N can be from 80 to 89
SYS_PING #N
<- SYS_PONG #N
 

// requests reading of free memory
SYS_MEM 99

// message with free memory
// VALUE is in bytes
<- SYS_MEM VALUE

// ask the Arduino whether it is configured already or not
SYS_CONFIG 99

// message with the "configured" state
// VALUE can be either 1 (yes) or 0 (no)
<- SYS_CONFIG VALUE

// tags the Arduino as configured
// doesn't really mean anything, but you can ask for this with the above command,
// useful when you restart your control app and wanna figure out, 
//     if you need to configure the Arduino or not
SYS_CONFIG 99 1
```

## US (ultrasonic measurements)
```
// registers a device 
// the #PIN_ECHO needs to be a pin with an interrupt
US_REG #PIN_SEND #PIN_ECHO

// sends this message when it thinks someone waved their hand in front of the sensor
// (basically something appeared 5 - 15 cm in front of it and then disappeared)
<- US #N WAVE

// asks for a measurement
US_READ #N 

// sends a the result of the measurement
// VALUE is in centimeters
<- US #N VALUE
```
