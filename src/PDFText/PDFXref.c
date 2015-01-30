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
*	@file		:	PDFXref.c
*	@brief		:	Reading blocks of xref
**/

	#include <PDFTextExtraction.h>

	C_MODE_START


	/***************************** Starting Prototypes ********************/
	void pdf_load_startxref(pdf_document *doc);
	void pdf_load_xref(pdf_document *doc, pdf_lexbuf *buf);
	/***************************** Ending Prototypes **********************/

	/***************************** Global Variables ********************/
	/***************************** Global Variables ********************/


	void pdf_load_xref(pdf_document *doc, pdf_lexbuf *buf PDFUnused){
		//Before loading of xref section its import of read
		//startxref position which is described at the end of file
		pdf_load_startxref(doc);

		printf("location of xref = %d\n", doc->startxref);
	}


	void pdf_load_startxref(pdf_document *doc){
		unsigned char buf[1024];
		int t, n;
		int i;

		//Going to the end of file
		pdf_seek(doc->file, 0, PDFSEEK_END);

		//Storing of total file size of inputfile
		doc->FileSize = pdf_tell(doc->file);

		//Storage of maximum rewind bytes
		t = (doc->FileSize ? doc->FileSize-1024 : 0);

		//Setting the cursor to read bytes
		pdf_seek(doc->file, t, PDFSEEK_SET);

		//Reading the bytes
		n = pdf_read(doc->file, buf, sizeof(buf));

		for (i = n - 9; i >= 0; i--){
			if (memcmp(buf + i, "startxref", 9) == 0){
				//Found the matching bytes
				i += 9;

				//Skipping any whites pace characters
				while (i < n && pdf_iswhite(buf[i])){
					i ++;
				}

				while (i < n && buf[i] >= '0' && buf[i] <= '9'){
					doc->startxref = doc->startxref * 10 + (buf[i++] - '0');
				}

				if (doc->startxref != 0){
					//Adding overheads if any available
					doc->startxref += doc->overhead;
					return;
				}
				break;
			}
		}

		printf("cannot find startxref\n");
		exit(0);
	}

	C_MODE_END
