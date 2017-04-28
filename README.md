Guide for Logic Analyzer RPi
==============

Logic Analyzer RPi is a program to use your Raspberry Pi as a Logic Analyzer. It allows you to capture logic states at the GPIO pins (using pull up then pull down resistors of the GPIO). The logic states of the GPIO can be measured up to few kHz, with a very large number of points. The captured data can be saved to or loaded from a file. Decoding can be performed according to specified protocol and results can be ploted in a PNG file.

This program has been made to make the implementation of new protocols as easy as possible.

I)Example of output 
==============
![logic states](https://github.com/superzerg/NMC9314_pi/blob/master/read.png)
Image obtained during two consecutive read instructions on a NMC9314 EEPROM memory using following command (loading read.dat capture file):

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
The order matters, for exemple with the microwire protocol:
- pins[0] acquires CS (Chip Select) of the studied device.
- pins[1] acquires CLK (clock) of the studied device.
- pins[2] acquires MOSI (Master Out Slave In) of the studied device.
- pins[3] acquires MISO (Master In Slave Out) of the studied device.

V) Install
===============
 1. install:
 - the C library for Raspberry Pi (http://www.open.com.au/mikem/bcm2835/index.html)
 - mgl library:
    sudo apt get install libmgl-dev libmgl-fltk5
 2. get the Logic Analyzer RPi files at https://github.com/superzerg/logic-analyzer
 	
	git clone https://github.com/superzerg/logic-analyzer.git
	
 3. compile with:
 
	./configure
	
	make
	
 4. test the program works correctly (no capture yet, just loading a file).
 
    make test

VI) Usage
===============

    ./logic_analyzer [-h] [-v level] [-w sec] [-l file | -c pins [-i ms] [-n npoint]] [-d protocol] [-s file] [-p file]
program displaying GPIO states over time and decoding protocols if supported. Protocols supported are :

- microwire: tested with an NMC9314 EEPROM.
- raw: no decoding is performed, but the logic state evolution is shown.
    
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
Copyright (C) 2012,2013 Renier Yves.

   Copying and distribution of this file, with or without modification,
are permitted in any medium without royalty provided the copyright
notice and this notice are preserved.  This file is offered as-is,
without warranty of any kind.

