CC=g++
CFLAGS=-c -Wall -O3 
#CFLAGS=-c -Wall -O3 -DNDEBUG
LDFLAGS=-lrt -lbcm2835 -lmgl -O3 

SOURCES=debug.cpp activity.cpp logic_analyzer.cpp message.cpp transition.cpp binary.cpp logic_input.cpp microwire.cpp protocol.cpp 
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
	sleep 1; sudo nice -n -20 ./logic_analyzer& sleep 2.7;sudo ../NMC9314B_programmer/NMC9314B_bit_banged -r -a 63 -b;sudo ../NMC9314B_programmer/NMC9314B_bit_banged -r -a 63 -b

view:
	display.im6 -update 1 test.png &
