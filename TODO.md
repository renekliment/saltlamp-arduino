# Priority
- US - do attachInterrupt nicely
	- multiple sensors
	- use the ultracool library?
- DS18B20
- the IR module doesn't work when using anything else
	- investigate IR libraries that use interrupts

# In the future
- diagnostics
- pin (+device) deregister (or register as sthing else)
- module for like a speaker / buzzer?
- investigate whether the String class causes any problems
	- with memory fragmentation / usage
	- does the .reserve() serve its purpose? do the boundaries cover all (or at least most) uses?
	- it is also the only thing@saltlamp that uses dynamic memory allocation
- include header files instead of .cpp files when it actually works