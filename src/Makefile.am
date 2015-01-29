#including our Headers
AM_CFLAGS = -I$(top_srcdir)/inc

#Our Cflags for compling
CFLAGS = -Wall -Wextra -Werror -Wimplicit-function-declaration -Wpointer-sign -pedantic\
	-Wcast-align -fstrict-overflow -Wstrict-overflow=5 -Wstrict-aliasing=5 \
	-Wmissing-prototypes -Wstrict-prototypes -Wwrite-strings -Wshadow -fcond-mismatch\
	-W -O3 -g -posix

MYLIBFLAGS = -no-undefined -export-symbols-regex -fpic -pie -shared -Wl,--kill-at -fomit-frame-pointer
windres = @WINDRES@

FILES = Main.c				

#Package to create
bin_PROGRAMS = PDFTextExtraction
PDFTextExtraction_CFLAGS = $(AM_CFLAGS)
PDFTextExtraction_SOURCES = $(FILES)

if PDFWINDRES
PDFTextExtraction_SOURCES += PDFTextExtraction.rc
endif

.rc.o :
	windres -O coff -o $@ $^
