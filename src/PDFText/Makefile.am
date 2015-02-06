# including of Header Files
AM_CFLAGS1 = -I$(top_srcdir)/inc -Wall -Wextra -Werror -Wimplicit-function-declaration -Wpointer-sign \
			-pedantic -pedantic -Wconversion -Wcast-align -fstrict-overflow -Wstrict-overflow=5 \
			-Wstrict-aliasing=5 -Wmissing-prototypes -Wstrict-prototypes -Wwrite-strings -Wshadow \
			-fcond-mismatch -W -O3 -posix


MYLIBFLAGS = -no-undefined -export-symbols-regex -fpic -pie -shared -Wl,--kill-at -fomit-frame-pointer

FILES = PDFInflate.c						\
		PDFPages.c							\
		PDFIndirect.c						\
		PDFObjects.c						\
		PDFParser.c							\
		PDFTrailer.c						\
		PDFXref.c							\
		PDFUtility.c						\
		PDFString.c							\
		PDFFileStream.c						\
		PDFLex.c							\
		PDFMem.c							\
		PDFFonts.c							\
		PDFFonts_TT.c						\
		PDFFonts_Table_OS2.c				\
		PDFFonts_Table_CMAP.c				\
		PDFTextExtraction.c



# Error Reporting Library
lib_LTLIBRARIES = libPDFTextExtraction.la

#For DLL/Shared Library
libPDFTextExtraction_la_SOURCES = $(FILES)
libPDFTextExtraction_la_CFLAGS = $(AM_CFLAGS1)
libPDFTextExtraction_la_LDFLAGS = $(MYLIBFLAGS)
