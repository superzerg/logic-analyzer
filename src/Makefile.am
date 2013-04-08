CFLAGS=-g -O2 -fstack-protector --param=ssp-buffer-size=4 -Wformat -Werror=format-security
AM_CPPFLAGS=-D_FORTIFY_SOURCE=2
AM_CXXFLAGS=-g -O2 -fstack-protector --param=ssp-buffer-size=4 -Wformat -Werror=format-security
AM_LDFLAGS=-lrt -lbcm2835 -lmgl -Wl,-z,relro

bin_PROGRAMS = logic_analyzer

logic_analyzer_SOURCES = logic_analyzer.cpp debug.cpp activity.cpp message.cpp transition.cpp binary.cpp logic_input.cpp microwire.cpp raw.cpp protocol.cpp debug.h activity.h message.h transition.h binary.h logic_input.h microwire.h raw.h protocol.h

CLEANFILES = config.status config.log src/*.o

MAINTAINERCLEANFILES = Makefile.in
