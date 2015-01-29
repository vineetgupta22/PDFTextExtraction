#including our Headers
AM_CFLAGS = -I$(top_srcdir)/inc

#Our Cflags for compling
CFLAGS = -Wall -Wextra -Werror -Wimplicit-function-declaration -Wpointer-sign -pedantic\
	-Wcast-align -fstrict-overflow -Wstrict-overflow=5 -Wstrict-aliasing=5 \
	-Wmissing-prototypes -Wstrict-prototypes -Wwrite-strings -Wshadow -fcond-mismatch\
	-W -O3 -g -posix

MYLIBFLAGS = -no-undefined -export-symbols-regex -fpic -pie -shared -Wl,--kill-at -fomit-frame-pointer
SUBDIRS = PDFText
windres = @WINDRES@

FILES = Main.c				

#Package to create
bin_PROGRAMS = PDFTextExt
PDFTextExt_CFLAGS = $(AM_CFLAGS)
PDFTextExt_SOURCES = $(FILES)
PDFTextExt_LDADD = $(top_srcdir)/src/PDFText/libPDFTextExtraction.la

if PDFWINDRES
PDFTextExt_SOURCES += PDFTextExtraction.rc
endif

.rc.o :
	windres -O coff -o $@ $^
