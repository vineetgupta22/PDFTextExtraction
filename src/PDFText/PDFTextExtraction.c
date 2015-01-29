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
*	@file		:	PDFTextExtraction.c
*	@brief		:	Library of PDF Text Extration line by line
**/

	#include <PDFTextExtraction.h>

	C_MODE_START


	/***************************** Starting Prototypes ********************/
	void pdf_init_document(pdf_document *doc);
	pdf_document *pdf_new_document(pdf_stream *file);
	PDFExport void PDFAPI PDFTextExtraction(const char *name);
	/***************************** Ending Prototypes **********************/

	/***************************** Global Variables ********************/
	/***************************** Global Variables ********************/


	PDFExport void PDFAPI PDFTextExtraction(const char *name){
		pdf_stream *file = NULL;
		pdf_document *doc;

		//Sending to Open the File for Processing
		file = pdf_open_file(name);

		//Init some of structures
		doc = pdf_new_document(file);

		//Started Reading of document
		pdf_init_document(doc);
	}



	pdf_document *pdf_new_document(pdf_stream *file){
		pdf_document *doc=NULL;

		doc=(pdf_document*)malloc(sizeof(pdf_document));
		memset(doc, 0, sizeof(pdf_document));

		//Sending to Init the lex elements for reading document
		pdf_lexbuf_init(&doc->lexbuf.base, PDFTextExt_LEXBUF_LARGE);
		doc->file = pdf_keep_stream(file);

		return doc;
	}

	void pdf_init_document(pdf_document *doc){
		//Starting the Reading of PDF File
		if ( doc ){
			printf("Started Reading of PDF File");
		}
	}

	C_MODE_END
