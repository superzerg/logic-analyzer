CC=g++
CFLAGS=-c -Wall -O3 
#CFLAGS=-c -Wall -O3 -DNDEBUG
LDFLAGS=-lrt -lbcm2835 -lmgl -O3 

SOURCES=logic_analyzer.cpp debug.cpp activity.cpp message.cpp transition.cpp binary.cpp logic_input.cpp microwire.cpp raw.cpp protocol.cpp
OBJECTS=$(SOURCES:.cpp=.o)
HEADERS=$(SOURCES:.cpp=.h)
EXECUTABLE=logic_analyzer

all: $(SOURCES) $(EXECUTABLE) 
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o: $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) *~
	
clean_all: 
	rm -f $(EXECUTABLE) $(OBJECTS) *~

test: $(EXECUTABLE) 
	sleep 1; ./logic_analyzer -l read.dat -d microwire -p read.png

bigtest: $(EXECUTABLE) 
	sleep 1; ./logic_analyzer -l readall.dat -d microwire -p readall.png
	
capture: $(EXECUTABLE) 
	sleep 1; sudo nice -n -20 ./logic_analyzer -c 15,14,4,27 -i 0.5 -n 2000 -s read.dat -d microwire -p read.png& sleep 2.5;sudo ../NMC9314B_programmer/NMC9314B_bit_banged -r -a 63 -b;sudo ../NMC9314B_programmer/NMC9314B_bit_banged -r -a 63 -b

bigcapture: $(EXECUTABLE) 
	sleep 1; sudo nice -n -20 ./logic_analyzer -c 15,14,4,27 -i 0.5 -n 70000 -s readall.dat -d microwire -p readall.png& sleep 2.5;sudo ../NMC9314B_programmer/NMC9314B_bit_banged -R -b

view:
	display.im6 -update 1 read.png &
	
bigview:
	display.im6 -update 1 readall.png &
