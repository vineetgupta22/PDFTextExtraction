/**
*	Copyright (c) 2008-2015, Cee Emm Infotech and/or its affiliates.
*	All rights are reserved.
*
*	Program Name:		PDFTextExtraction
*	Program Owner:		Cee Emm Infotech
*	Chief Developer:	Vineet Gupta
*
*	contact: vineetgupta22@gmail.com
*	Address: Cee Emm Infotech, 605, Sector 10D, Chandgiarh [INDIA]
*
**/


/**
*	@file		:	PDFLex.c
*	@brief		:	Functions PDF Lex Elements processing
**/

	#include <PDFTextExtraction.h>

	C_MODE_START


	/***************************** Starting Prototypes ********************/
	void pdf_lexbuf_init(pdf_lexbuf *lex_buffer, int size);
	/***************************** Ending Prototypes **********************/

	/***************************** Global Variables ********************/
	/***************************** Global Variables ********************/


	void pdf_lexbuf_init(pdf_lexbuf *lex_buffer, int size){
		lex_buffer->size=size;
		lex_buffer->base_size=size;
		lex_buffer->len=0;
		lex_buffer->scratch = &lex_buffer->buffer[0];
	}

	C_MODE_END
