# Priority
- the IR module doesn't work when using anything else
	- investigate IR libraries that use interrupts
	- it actually sometimes works .. needs further investigation
- implement various checks for input commands
- implement using analog pins as digital
- think about implementing
	- shift registers
	- RGB LED stuff
- revise and document TEMP error states

# In the future
- US - do attachInterrupt nicely
	- multiple sensors
	- use the ultracool library?
- multiple Dallas sensors
- diagnostics
- pin (+device) deregister (or register as sthing else)
- module for like a speaker / buzzer?
- investigate whether the String class causes any problems
	- with memory fragmentation / usage
	- does the .reserve() serve its purpose? do the boundaries cover all (or at least most) uses?
- include header files instead of .cpp files when it actually works