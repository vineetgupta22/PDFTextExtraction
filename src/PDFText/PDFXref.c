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
	int pdf_xref_size(pdf_document *doc, pdf_lexbuf *buf);
	void pdf_load_xref(pdf_document *doc, pdf_lexbuf *buf);
	pdf_trailer *pdf_read_xref(pdf_document *doc, int offset, pdf_lexbuf *buf);
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


	/**
	*	@fn			pdf_load_xref_section(pdf_document *doc, int offset,
	*				pdf_lexbuf *buf, offsets_list *offsets)
	*	@param[in]	doc		PDF Document for storage
	*	@param[in]	offset	Beginning offset of xref section
	*	@param[in]	buf		Lex Buffer for doing Lex Parser
	*	@param[in]	offsets	List of all offset upto now read
	*	@brief		Function parse the current xref section and trailer and
	*	would return the previous section offsets.
	**/
	int pdf_load_xref_section(pdf_document *doc, int offset, pdf_lexbuf *buf, offsets_list *offsets){
		int i;
		pdf_trailer	*trailer;

		/* for avoid potential infinite recursion sections */
		for (i = 0; i < offsets->len; i ++){
			if (offsets->list[i] == offset){
				break;
			}
		}
		if (i < offsets->len){
			printf("ignoring xref recursion with offset %d\n", offset);
			exit(0);
		}
		if (offsets->len == offsets->max){
			printf("Increase the offsets_list_left>10\n");
			exit(0);
		}

		//Recording the current offset as first offset read
		offsets->list[offsets->len++] = offset;

		//Reading the Trailer Section of the document
		trailer = pdf_read_xref(doc, offset, buf);

		if ( trailer ){}
		//temprory
		return 0;
	}

	pdf_trailer *pdf_read_xref(pdf_document *doc, int offset, pdf_lexbuf *buf){
		pdf_trailer *trailer=NULL;
		int c;

		//Setting the offset to xref section
		pdf_seek(doc->file, offset, PDFSEEK_SET);

		while (pdf_iswhite(pdf_peek_byte(doc->file)))
			pdf_read_byte(doc->file);

		//looking at next byte
		c = pdf_peek_byte(doc->file);
		if (c == 'x'){
			int xref_len;

			//Before proceeding further we must know the size of xref section
			//actually used in file.
			xref_len=pdf_xref_size(doc, buf);

			printf("Total size of xref=%d\n\n", xref_len);
		}else{
			printf("cannot recognize xref format\n");
			exit(0);
		}

		return trailer;
	}


	int pdf_xref_size(pdf_document *doc, pdf_lexbuf *buf){
		int size=0, ofs, c, len, t;
		char *s;

		//Recording the current position
		ofs = pdf_tell(doc->file);

		pdf_read_line(doc->file, buf->scratch, buf->size);
		if (strncmp(buf->scratch, "xref", 4) != 0){
			printf("cannot find the xref tag\n");
			exit(0);
		}

		while(1){
			c = pdf_peek_byte(doc->file);

			//not a xref section
			if (!(c >= '0' && c <= '9')){
				break;
			}

			pdf_read_line(doc->file, buf->scratch, buf->size);
			s = buf->scratch;
			pdf_strsep(&s, " "); 				/* ignore ofs */
			len=pdf_atoi(pdf_strsep(&s, " "));	//Length of xref section

			if (len < 0){
				printf("xref range marker must be positive\n");
				exit(0);
			}

			t=pdf_tell(doc->file);
			if (len > (INT_MAX - t) / 20){
				printf("xref has too many entries can't handle all\n");
				exit(0);
			}

			//Jumping xref section
			pdf_seek(doc->file, t + (20 * len), PDFSEEK_SET);
		}

		unsigned char buff[1024];
		int n, i;
		n = pdf_read(doc->file, buff, sizeof(buff));
		for (i=0; i<n; i++){
			if ( (memcmp(buff + i, "Size", 4) == 0) ){
				i += 4;
				while (i < n && (buff[i] <= 32 || buff[i] >= 127)){
					i++;
				}
				size=0;
				while (i < n && buff[i] >= '0' && buff[i] <= '9'){
					size=size * 10 + (buff[i++] - '0');
				}
				break;
			}
		}

		//Restoring the offset recorded
		pdf_seek(doc->file, ofs, PDFSEEK_SET);
		return size;
	}

	C_MODE_END
