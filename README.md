Guide for Logic Analyzer RPi
==============

I)Exemple of output 
==============
![logic states](read.png)
Image otained durring two consecutive read instructions on a NMC9314 EEPROM memory using following command:

	make test

II) Dependance
==============
It use the C library for Raspberry Pi (http://www.open.com.au/mikem/bcm2835/index.html)

III) Disclamer
===============
This program has been made for personal use, it works for me, but I offer no waranty as this program may (unlikely) damage your raspberry pi, your memory and/or any electronic device connected. Use it at your own risk.

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

VI) Install
===============
1. install the C library for Raspberry Pi (http://www.open.com.au/mikem/bcm2835/index.html)
2. get the Logic Analyser RPi files at https://github.com/superzerg/logic-analyzer
3. compile with:
	make

VII) More Information
===============
More information is availble with the developper guide (http://superzerg.github.com/logic-analyzer/index.html)

VIII) Licence
===============
Copyright (C) 1994, 1995, 1996, 1999, 2000, 2001, 2002, 2004, 2005,
2006, 2007, 2008, 2009 Free Software Foundation, Inc.

   Copying and distribution of this file, with or without modification,
are permitted in any medium without royalty provided the copyright
notice and this notice are preserved.  This file is offered as-is,
without warranty of any kind.

