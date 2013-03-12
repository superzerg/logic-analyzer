Guide for Logic Analyzer RPi
==============

I)Example of output 
==============
![logic states](read.png)
Image obtained during two consecutive read instructions on a NMC9314 EEPROM memory using following command (loading test.dat capture file):

	make test

II) Dependence 
==============
It use the C library for Raspberry Pi [http://www.open.com.au/mikem/bcm2835](http://www.open.com.au/mikem/bcm2835/index.html)

III) Disclaimer
===============
This program has been made for personal use, it works for me, but I offer no warranty as this program may (unlikely) damage your raspberry pi, your memory and/or any electronic device connected. Use it at your own risk.

IV) Connection
===============

If you have to make measurement on a 5V circuit, this is a way to avoid toasting your GPIO.

	RASPBERRY PI              74F126N
	 ______________       ________________________
	|              |     |                       |
	|              |  /--|3Y (pin 5)   3A (pin 6)|<--- INPUT
	|              |  |  |_______________________|
	|              | | | 
	|              | |R|
	|              | |1|
	|              | |_|
	|              |  | 
	|         GPIOi|<-|
	|______________|  |
	                 | |
	                 |R|
	                 |2|
	                 |_|
	                  |
	                 GND 
	R1=32 kohms
	R2=68 kohms

On a 3V3 circuit you can connect direclty your input to the GPIO pin.
As the GPIO pins number orderring is at least confusing, you can print the following image corrsponding to your RPi version (let your mouse on an image to see the version). 
Then apply it on GPIO pins (from http://www.doctormonk.com/2013/02/raspberry-pi-and-breadboard-raspberry.html).
PDF files are also availble there for easier printing.

![revison 1 boards](http://3.bp.blogspot.com/-M0E1IVgNnlc/USI5afQSoqI/AAAAAAAAEhA/QnwqdTqGNE0/s320/raspberry+leaf.png "Rev. 1 board")
![revison 2 boards](http://1.bp.blogspot.com/-s2VVUs3YO5E/USTWSZq1FzI/AAAAAAAAEqk/iRyL0OY6ZWU/s320/raspberry+leaf+r2.png "Rev. 2 board")

The numbers next to the GPIO pins used to acquire data are the one to use in [pins](http://superzerg.github.com/logic-analyzer/classlogic__input.html#a691df164430ee863f12e0aaaeb7510a7) array.
The order matter, for exemple with the microwire protocol:
- pins[0] acquires CS (Chip Select) of the studied device.
- pins[1] acquires CLK (clock) of the studied device.
- pins[2] acquires MOSI (Master Out Slave In) of the studied device.
- pins[3] acquires MISO (Master In Slave Out) of the studied device.

V) Install
===============
1. install the C library for Raspberry Pi (http://www.open.com.au/mikem/bcm2835/index.html)
2. get the Logic Analyzer RPi files at https://github.com/superzerg/logic-analyzer
3. compile with:
	make


VI) Usage
===============

    ./logic_analyzer [-h] [-v level] [-w sec] [-l file | -c pins [-i ms] [-n npoint]] [-d protocol] [-s file] [-p file]
program displaying GPIO states over time and decoding protocols if supported. Protocols supported are :

- microwire
- raw
    
program parameters are:

- -h|--help : this help.
- -l|--load file : Use data in binary file as captured data.
- -c|--capture pins: pins to use durring the capture. Must be the GPIO numbers separated by \',\' or \' \'. Space may be used if le string pins is protected by \"\". ex 14,15,27,4 or \"14 15 27 4\". Order may be inportant if --decode is used depending on the protocol.
- -i|--interval ms: interval in ms between 2 captures (default is 0.5 ms wich is the recomended minimum). For low intervals (ms<10), the interval can be sometime higher if the RPi is busy durring capture.
- -n|--npoint npoint: number of data point to capture (default 2000).
- -d|--decode protocol: decode the captured data using the specified protocol. Some protocols may need a special order for the pins given to the --capture option.
- -s|--save file : save captured data to binary file.
- -p|--plot file : plot captured data to PNG file.
- -w|--wait sec : time in second to wait before the capture.
- -v|--verbose level: set the verbosity to level (0 to 4, higher means more messages,default is 3).


VII) More Information
===============
More information is available with the developer's guide (http://superzerg.github.com/logic-analyzer/index.html)

VIII) License
===============
Copyright (C) 1994, 1995, 1996, 1999, 2000, 2001, 2002, 2004, 2005,
2006, 2007, 2008, 2009 Free Software Foundation, Inc.

   Copying and distribution of this file, with or without modification,
are permitted in any medium without royalty provided the copyright
notice and this notice are preserved.  This file is offered as-is,
without warranty of any kind.

