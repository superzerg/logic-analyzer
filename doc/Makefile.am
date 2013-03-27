if HAVE_DOXYGEN
directory3 = $(top_srcdir)/doc/man/man3
directory1 = $(top_srcdir)/doc/man/man1

dist_man_MANS = $(directory3)/activity.3  $(directory3)/binary.3 $(directory3)/logic_input.3 $(directory3)/message.3 $(directory3)/microwire.3  $(directory3)/protocol.3 $(directory3)/raw.3 $(directory3)/transition.3  

man_MANS=$(directory1)/logic_analyzer.1

$(directory3)/activity.3 : doxyfile.stamp

$(directory3)/binary.3: doxyfile.stamp

$(directory3)/logic_input.3: doxyfile.stamp

$(directory3)/message.3: doxyfile.stamp

$(directory3)/microwire.3 : doxyfile.stamp

$(directory3)/protocol.3: doxyfile.stamp

$(directory3)/raw.3: doxyfile.stamp

$(directory3)/transition.3: doxyfile.stamp

doxyfile.stamp: 
	$(DOXYGEN) Doxyfile
	echo Timestamp> doxyfile.stamp

CLEANFILES = doxyfile.stamp Doxyfile

all-local: doxyfile.stamp
clean-local:
	rm -rf $(top_srcdir)/doc/man/man3 $(top_srcdir)/doc/html $(top_srcdir)/doc/latex 
endif

