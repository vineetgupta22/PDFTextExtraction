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
	void pdf_next_xref(pdf_document *doc);
	void pdf_load_startxref(pdf_document *doc);
	void pdf_load_xref(pdf_document *doc, pdf_lexbuf *buf);
	void pdf_load_xref_last(pdf_document *doc, int offset, pdf_lexbuf *buf, int ReadPrevious);
	int pdf_load_xref_section(pdf_document *doc, int offset, pdf_lexbuf *buf, offsets_list *offsets);
	/***************************** Ending Prototypes **********************/

	/***************************** Global Variables ********************/
	static int offsets_list_left;
	/***************************** Global Variables ********************/


	void pdf_next_xref(pdf_document *doc){
		if ( offsets_list_left ){
			pdf_xref *xref;
			if ( offsets_list_left == 10 ){
				doc->xref_sections = (pdf_xref *)malloc(sizeof(pdf_xref)*10);
				memset(doc->xref_sections,0, sizeof(pdf_xref)*10);

				//Adding Memory allocation size
				MallocSize+=(sizeof(pdf_xref)*10);
			}
			doc->total_xref_sections++;
			xref = &doc->xref_sections[doc->total_xref_sections - 1];
			xref->subsec = NULL;
			xref->objects = 0;
			xref->trailer = NULL;
		}else{
			printf("Maximum Numbers of sections read");
			exit(0);
		}
		offsets_list_left--;
	}

	void pdf_load_xref_last(pdf_document *doc, int offset, pdf_lexbuf *buf, int ReadPrevious){
		offsets_list all_offsets;

		//Section Left
		offsets_list_left=10;

		//Currently we have length
		all_offsets.len = 0;

		//Maximum Number of list we can store
		all_offsets.max=10;

		all_offsets.list	=	(int *)malloc(sizeof(int)*(unsigned)all_offsets.max);
		memset(all_offsets.list, 0,  (sizeof(int)*((unsigned)all_offsets.max)));

		//Adding Memory allocation size
		MallocSize+=(sizeof(int)*10);

		while ( offset ){
			pdf_next_xref(doc);
			offset = pdf_load_xref_section(doc, offset, buf, &all_offsets);
			if ( ! ReadPrevious ){
				break;
			}
		}

		//Freeing the all offsets lists
		free(all_offsets.list);

		//Removing Memory allocation size
		MallocSize-=(sizeof(int)*10);
	}


	void pdf_load_xref(pdf_document *doc, pdf_lexbuf *buf){
		//Before loading of xref section its import of read
		//startxref position which is described at the end of file
		pdf_load_startxref(doc);

		//We have not the starting point of startxref not we need to
		//read the startxref sections
		pdf_load_xref_last(doc, doc->startxref, buf, 1);
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


	int pdf_load_xref_section(pdf_document *doc PDFUnused, int offset PDFUnused, pdf_lexbuf *buf PDFUnused, offsets_list *offsets PDFUnused){

		//temprory
		return 0;
	}

	C_MODE_END
