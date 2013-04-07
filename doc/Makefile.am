directory3= $(top_srcdir)/doc/man/man3
directory1= $(top_srcdir)/doc/man/man1
directoryinfo= $(top_srcdir)/doc
prefix3=logic_analyzer

info_TEXINFOS=logic_analyzer.texi
man_MANS=$(directory1)/$(prefix3).1

if HAVE_DOXYGEN

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

	
CLEANFILES = doxyfile.stamp Doxyfile

all-local: doxyfile.stamp
clean-local:
	rm -rf $(top_srcdir)/doc/man/man3 $(top_srcdir)/doc/html $(top_srcdir)/doc/latex 
endif

