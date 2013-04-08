AUTOMAKE_OPTIONS = gnu
SUBDIRS = src doc

MAINTAINERCLEANFILES= Makefile.in aclocal.m4 config.guess config.sub configure depcomp install-sh missing

EXTRA_DIST=gen_changelog.sh

dist_pkgdata_DATA = examples/test.dat

clean-local:
	rm -fr autom4te.cache
	
dist-hook: 
	bash $(top_srcdir)/gen_changelog.sh $(top_srcdir) $(distdir)


