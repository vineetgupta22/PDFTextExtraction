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
*	@file		:	PDFTextExtractionExtern.h
*	@brief		:	External Functions
**/

	#ifndef		_PDFTextExtractionExtern_
		/**
		*	@def	:	_PDFTextExtractionExtern_
		*	@brief	:	Header Including Parameter for Only Once.
		**/
		#define			_PDFTextExtractionExtern_

		//doing alignment of 4 bytes as required by some systems
		#pragma		pack(push, 4)


		//Function defined under src/PDFTextExtraction.c
		extern PDFExport void PDFAPI PDFTextExtraction(const char *name);


		//Function defined under src/PDFFileStream.c
		extern pdf_stream *pdf_open_file(const char *name);
		extern pdf_stream *pdf_keep_stream(pdf_stream *stm);


		//Function defined under src/PDFLex.c
		extern void pdf_lexbuf_init(pdf_lexbuf *lex_buffer, int size);


		//Endling of alignment
		#pragma		pack(pop)
	#endif