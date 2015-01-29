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
	void pdf_file_version(pdf_document *doc);
	void pdf_init_document(pdf_document *doc);
	pdf_document *pdf_new_document(pdf_stream *file);
	void pdf_check_start_overheads(pdf_document *doc);
	PDFExport void PDFAPI PDFTextExtraction(const char *name);
	/***************************** Ending Prototypes **********************/

	/***************************** Global Variables ********************/
	unsigned long MallocSize;
	/***************************** Global Variables ********************/


	PDFExport void PDFAPI PDFTextExtraction(const char *name){
		pdf_stream *file = NULL;
		pdf_document *doc;

		//For Keeping the size of malloc done and freed.
		MallocSize=0;

		//Sending to Open the File for Processing
		file = pdf_open_file(name);

		//Init some of structures
		doc = pdf_new_document(file);

		//Started Reading of document
		pdf_init_document(doc);

		printf("Total Memory Allocation done=%lu\n\n", MallocSize);
	}



	pdf_document *pdf_new_document(pdf_stream *file){
		pdf_document *doc=NULL;

		doc=(pdf_document*)malloc(sizeof(pdf_document));
		memset(doc, 0, sizeof(pdf_document));
		MallocSize+=sizeof(pdf_document);

		//Sending to Init the lex elements for reading document
		pdf_lexbuf_init(&doc->lexbuf.base, PDFTextExt_LEXBUF_LARGE);
		doc->file = pdf_keep_stream(file);

		return doc;
	}


	void pdf_check_start_overheads(pdf_document *doc){
		int i=0;

		pdf_seek(doc->file, 0, PDFSEEK_SET);

		while (pdf_iswhite(pdf_peek_byte(doc->file))){
			pdf_read_byte(doc->file);
			i++;
		}

		doc->overhead=i;
	}


	void pdf_file_version(pdf_document *doc){
		//Before Reading file version sometimes it happens that the file
		//doesn't start with  required %%PDF tag thus he have to check
		//the starting overheads
		pdf_check_start_overheads(doc);

		char buf[20];

		pdf_read_line(doc->file, buf, sizeof buf);
		if (memcmp(buf, "%PDF-", 5) != 0){
			printf("cannot recognize version marker\n");
			exit(0);
		}

		doc->version = (int)(10 * (pdf_atof(buf+5) + 0.05));
	}


	void pdf_init_document(pdf_document *doc){
		//First of all we need to set the pdf version
		pdf_file_version(doc);

		//Starting the Reading of PDF File
		if ( doc ){
			printf("Started Reading of PDF File\n\n");
		}
	}

	C_MODE_END
