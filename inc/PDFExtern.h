/**
*	Copyright (c) 2008-2015, Cee Emm Infotech and/or its affiliates.
*	All rights are reserved.
*
*	Program Name:		PDFTextExtraction
*	Purpose of Program:	Extraction of Text from PDF Files
*	Program Owner:		Cee Emm Infotech
*	Chief Developer:	Vineet Gupta
*	
*	contact: vineetgupta22@gmail.com
*	Address: Cee Emm Infotech, 605, Sector 10D, Chandgiarh [INDIA]
*
**/

/**
*	@file		:	PDFExtern.h
*	@brief		:	External Functions
**/

	#ifndef		_PDFTextExtractionExtern_
		/**
		*	@def		_PDFTextExtractionExtern_
		*	@brief		Header Including Parameter for Only Once.
		**/
		#define			_PDFTextExtractionExtern_

		//doing alignment of 4 bytes as required by some systems
		#pragma		pack(push, 4)


		//Function defined under src/PDFTextExtraction.c
		extern PDFExport void PDFAPI PDFTextExtraction(const char *name);


		//Function defined under src/PDFFileStream.c
		extern int pdf_tell(pdf_stream *stm);
		extern int pdf_read_byte(pdf_stream *stm);
		extern int pdf_peek_byte(pdf_stream *stm);
		extern int pdf_next(pdf_stream *stm, int n);
		extern pdf_stream *pdf_open_file(const char *name);
		extern pdf_stream *pdf_keep_stream(pdf_stream *stm);
		extern void pdf_read_line(pdf_stream *stm, char *mem, int n);
		extern void pdf_seek(pdf_stream *stm, int offset, int whence);
		extern int pdf_read(pdf_stream *stm, unsigned char *buf, int len);


		//Function defined under src/PDFLex.c
		extern void pdf_lexbuf_init(pdf_lexbuf *lex_buffer, int size);


		//Functions defined under src/PDFUtility.c
		extern int pdf_iswhite(int ch);


		//Functions defined under src/PDFString.c
		extern int pdf_atoi(const char *s);
		extern float pdf_atof(const char *s);
		extern char *pdf_strsep(char **str, const char *delim);


		//Functions defined user src/PDFXref.c
		extern void pdf_load_xref(pdf_document *doc, pdf_lexbuf *buf);

		//Endling of alignment
		#pragma		pack(pop)
	#endif