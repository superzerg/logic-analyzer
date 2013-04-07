info_TEXINFOS=logic_analyzer.texi
man_MANS=$(top_srcdir)/doc/man/man1/logic_analyzer.1

MAINTAINERCLEANFILES = logic_analyzer.info stamp-vti version.texi Makefile.in

if HAVE_DOXYGEN

directory3= $(top_srcdir)/doc/man/man3
prefix3=logic_analyzer
doxyman3=activity.3 binary.3 logic_input.3 message.3 microwire.3 protocol.3 raw.3 transition.3 


dist_man_MANS = $(directory3)/$(prefix3)_activity.3  $(directory3)/$(prefix3)_binary.3 $(directory3)/$(prefix3)_logic_input.3 $(directory3)/$(prefix3)_message.3 $(directory3)/$(prefix3)_microwire.3  $(directory3)/$(prefix3)_protocol.3 $(directory3)/$(prefix3)_raw.3 $(directory3)/$(prefix3)_transition.3 

$(directory3)/$(prefix3)_activity.3 : doxyfile.stamp

$(directory3)/$(prefix3)_binary.3: doxyfile.stamp

$(directory3)/$(prefix3)_logic_input.3: doxyfile.stamp

$(directory3)/$(prefix3)_message.3: doxyfile.stamp

$(directory3)/$(prefix3)_microwire.3 : doxyfile.stamp

$(directory3)/$(prefix3)_protocol.3: doxyfile.stamp

$(directory3)/$(prefix3)_raw.3: doxyfile.stamp

$(directory3)/$(prefix3)_transition.3: doxyfile.stamp

doxyfile.stamp: 
	$(DOXYGEN) Doxyfile
	echo Timestamp> doxyfile.stamp
	for file3 in $(doxyman3); do \
		mv $(directory3)/$$file3 $(directory3)/$(prefix3)_$$file3; \
	done

	
CLEANFILES = doxyfile.stamp 
DISTCLEANFILES = Doxyfile

all-local: doxyfile.stamp
clean-doxygen:
	rm -rf $(top_srcdir)/doc/man/man3 $(top_srcdir)/doc/html $(top_srcdir)/doc/latex 
clean-local: clean-doxygen
endif

