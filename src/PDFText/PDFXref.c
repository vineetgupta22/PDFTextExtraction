/**
*	@file		:	PDFXref.c
*	@brief		:	Reading blocks of xref
**/

	#include <PDFTextExtraction.h>

	C_MODE_START


	/***************************** Starting Prototypes ********************/
	void pdf_next_xref(pdf_document *doc);
	void pdf_read_version(pdf_document *doc);
	void pdf_load_startxref(pdf_document *doc);
	void pdf_contains_binary(pdf_document *doc);
	void pdf_start_overheads(pdf_document *doc);
	void extend_xref_index(pdf_document *doc, int newlen);
	void pdf_load_xref(pdf_document *doc, pdf_lexbuf *buf);
	pdf_trailer *pdf_read_xref(pdf_document *doc, pdf_lexbuf *buf);
	pdf_xref_entry *pdf_xref_find_subsection(pdf_document *doc, int start, int length);
	pdf_trailer *pdf_read_xref_section(pdf_document *doc, int offset, pdf_lexbuf *buf);
	void pdf_load_xref_last(pdf_document *doc, int offset, pdf_lexbuf *buf, int ReadPrevious);
	int pdf_load_xref_section(pdf_document *doc, int offset, pdf_lexbuf *buf, offsets_list *offsets);
	/***************************** Ending Prototypes **********************/

	/***************************** Global Variables ********************/
	//offset list of xref sections for reading
	static int offsets_list_left;
	/***************************** Global Variables ********************/


	void extend_xref_index(pdf_document *doc, int newlen){
		int i;

		if ( !doc->max_xref_len ){
			doc->xref_index = (int *)malloc(sizeof(int)*(unsigned)newlen);
			memset(doc->xref_index, 0, sizeof(int)*(unsigned)newlen);
			MallocSize+=(sizeof(int)*(unsigned)newlen);
			for (i = doc->max_xref_len; i < newlen; i++){
				doc->xref_index[i] = 0;
			}
			doc->max_xref_len = newlen;
		}else{
			printf("already malloc done\n");
			exit(0);
		}
	}


	/**
	*	@fn 	pdf_read_xref_section(pdf_document *doc, int offset,
	*			pdf_lexbuf *buf)
	*	@param[in]	doc		PDF Document for storage
	*	@param[in]	start	Starting Number of sub section
	*	@param[in]	len		Length of xref sub section
	*	@brief		Different cases here.
	*	1)	We might be asking for a subsection (or a subset of a subsection)
	*	that we already have -  Just return it.
	*	2)	We might be asking for a completely new subsection - Create it
	*	and return it.
	*	3)	We might have an overlapping one - Create a 'solid' subsection and
	*	return that.
	**/
	pdf_xref_entry *pdf_xref_find_subsection(pdf_document *doc, int start, int length){
		pdf_xref *xref = &doc->xref_sections[doc->total_xref_sections-1];
		pdf_xref_subsec *sub=NULL;

		/* Sanity check for case 1 and 3*/
		for (sub = xref->subsec; sub != NULL; sub = sub->next){
			if (start >= sub->start && start + length <= sub->start + sub->len){
				printf("First case found\n");
				exit(0);
			}
			if (start + length > sub->start && start <= sub->start + sub->len){
				//this is third case
				break;
			}
		}

		int new_max = xref->objects;
		if (new_max < start + length){
			new_max = start + length;
		}

		if (sub == NULL){
			sub = (pdf_xref_subsec*)malloc(sizeof(pdf_xref_subsec));
			memset(sub, 0, sizeof(pdf_xref_subsec));
			MallocSize+=sizeof(pdf_xref_subsec);

			sub->table = (pdf_xref_entry*)malloc((unsigned)length*sizeof(pdf_xref_entry));
			memset(sub->table, 0, ((unsigned)length*sizeof(pdf_xref_entry)));
			MallocSize+=((unsigned)length*sizeof(pdf_xref_subsec));

			sub->start = start;
			sub->len = length;
			sub->next = xref->subsec;
			xref->subsec = sub;

			xref->objects = new_max;

			if (doc->max_xref_len < new_max){
				extend_xref_index(doc, new_max);
			}
		}else{
			printf("case 3 provide code\n");
			exit(0);
		}

		return &sub->table[start-sub->start];
	}

	/**
	*	@fn			pdf_read_xref(pdf_document *doc, pdf_lexbuf *buf)
	*	@param[in]	doc		PDF Document for storage
	*	@param[in]	buf		Lex Buffer for doing Lex Parser
	*	@brief		The offset in the file is already set we have to just
	*	parse the xref section and return
	**/
	pdf_trailer *pdf_read_xref(pdf_document *doc, pdf_lexbuf *buf){
		pdf_trailer *trailer=NULL;
		int c, offsets, len, i;
		char *s;

		//Currently it is unknown that how many sub sections as in xref table
		//so first we need to know the size of xref which is described in trailer
		int xref_len = pdf_xref_size_from_trailer(doc, buf);

		pdf_read_line(doc->file, buf->scratch, buf->size);

		//Checking the line must be xref
		if (strncmp(buf->scratch, "xref", 4) != 0){
			printf("cannot find the xref tag\n");
			exit(0);
		}

		while (1){
			//peek a byte
			c = pdf_peek_byte(doc->file);

			//xref sub section must start with 0 to 9
			if (!(c >= '0' && c <= '9')){
				break;
			}

			//reading line
			pdf_read_line(doc->file, buf->scratch, buf->size);
			s = buf->scratch;

			offsets = pdf_atoi(pdf_strsep(&s, " "));
			len = pdf_atoi(pdf_strsep(&s, " "));

			if (s && *s != '\0'){
				/* broken pdfs where the section is not on a separate line */
				printf("borken xref sub section\n");
				exit(0);
			}

			if (offsets < 0){
				printf("out of range object num in xref: %d\n", offsets);
				exit(0);
			}

			/* broken pdfs where size in trailer undershoots entries in xref sections */
			if (offsets + len > xref_len){
				printf("broken xref section, proceeding anyway\n");
			}

			pdf_xref_entry *table;
			table = pdf_xref_find_subsection(doc, offsets, len);
			if ( table ){
				for (i = offsets; i < offsets + len; i++){
					pdf_xref_entry *entry = &table[i-offsets];
					int n = pdf_read(doc->file, (unsigned char *)buf->scratch, 20);
					if (n != 20){
						printf("unexpected EOF in xref table\n");
						exit(0);
					}
					if (entry->type == 0 ){
						s = buf->scratch;

						/* broken pdfs where line start with white space */
						while (*s != '\0' && pdf_iswhite(*s)){
							s++;
						}

						entry->offsets = atoi(s) + doc->overhead;
						entry->gen = (short unsigned)atoi(s + 11);
						entry->type = s[17];
						if (s[17] != 'f' && s[17] != 'n' && s[17] != 'o'){
							printf("unexpected xref type: %#x (%d %d R)\n", s[17], i, entry->gen);
							exit(0);
						}
					}
				}
			}
		}

		return trailer;
	}


	/**
	*	@fn			pdf_read_xref_section(pdf_document *doc, int offset,
	*				pdf_lexbuf *buf)
	*	@param[in]	doc		PDF Document for storage
	*	@param[in]	offset	Beginning offset of xref section
	*	@param[in]	buf		Lex Buffer for doing Lex Parser
	*	@brief		we set the offset to the provided location and start parsing
	*	xref section including trailer.
	**/
	pdf_trailer *pdf_read_xref_section(pdf_document *doc, int offset, pdf_lexbuf *buf){
		pdf_trailer *trailer=NULL;
		int c;

		//setting the offset to xref offset provided
		pdf_seek(doc->file, offset, PDFSEEK_SET);

		//skipping white space if any
		while (pdf_iswhite(pdf_peek_byte(doc->file))){
			pdf_read_byte(doc->file);
		}

		//Check if the first byte is x or between 0 to 9 or error
		c = pdf_peek_byte(doc->file);
		if (c == 'x'){
			trailer = pdf_read_xref(doc, buf);
		}else{
			printf("x not found it is %c|%d\n", c, c);
			exit(0);
		}
		return trailer;
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

		//We have to return the previous xref section offsets for that we have
		//to parse the trailer and as per Manual of PDF. Trailer must contain
		//the reference offset bytes of Previous xref if any available in document.
		trailer=pdf_read_xref_section(doc, offset, buf);

		if ( ! trailer ){
			printf("trailer in PDF not found\n");
			exit(0);
		}

		//temprory
		return 0;
	}

	/**
	*	@fn							pdf_next_xref(pdf_document *doc)
	*	@param[in]	doc				PDF Document to be filled
	*	@brief				Memory allocation for reading xref sections.
	**/
	void pdf_next_xref(pdf_document *doc){
		if ( offsets_list_left ){
			pdf_xref *xref;
			if ( offsets_list_left == 25 ){
				doc->xref_sections = (pdf_xref *)malloc(sizeof(pdf_xref)*25);
				memset(doc->xref_sections,0, sizeof(pdf_xref)*25);

				//Adding Memory allocation size
				MallocSize+=(sizeof(pdf_xref)*25);
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

	/**
	*	@fn							pdf_load_xref_last(pdf_document *doc, int offset,
	*								pdf_lexbuf *buf, int ReadPrevious)
	*	@param[in]	doc				PDF Document to be filled
	*	@param[in]	offset			Location of Last Updated xref section
	*	@param[in]	buf				For Procession Lexical conventions of PDF
	*	@param[in]	ReadPrevious	To read the previous xref sections or not.
	*	@brief				Going to load all the valid objects in the xref
	*	section. In case of incremental updates the separate xref section would
	*	be used to each update xref section.
	**/
	void pdf_load_xref_last(pdf_document *doc, int offset, pdf_lexbuf *buf, int ReadPrevious){
		offsets_list all_offsets;

		//Section Left
		offsets_list_left=25;

		//Currently we have length
		all_offsets.len = 0;

		//Maximum Number of list we can store
		all_offsets.max=25;

		all_offsets.list	=	(int *)malloc(sizeof(int)*(unsigned)all_offsets.max);
		memset(all_offsets.list, 0,  (sizeof(int)*((unsigned)all_offsets.max)));

		//Adding Memory allocation size
		MallocSize+=(sizeof(int)*25);

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
		MallocSize-=(sizeof(int)*25);
	}

	/**
	*	@fn					pdf_load_startxref(pdf_document *doc)
	*	@param[in]	doc		PDF Document to be filled
	*	@brief			Reading the tag startxref. According to PDF Manual
	*	it is important the startxref should be mentioned above %%EOF tag.
	*	As it will point to the offset position of xref.
	**/
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
	*	@fn					pdf_contains_binary(pdf_document *doc)
	*	@param[in]	doc		PDF Document to be filled
	*	@brief		If a PDF file contains binary data, as most do the
	*	header line shall be immediately followed by a comment line
	*	containing at least four binary characters whose codes are 128 or
	*	greater
	**/
	void pdf_contains_binary(pdf_document *doc){
		while (pdf_iswhite(pdf_peek_byte(doc->file))){
			pdf_read_byte(doc->file);
		}
		int i, c;

		if ( pdf_peek_byte(doc->file) == 37 ){
			pdf_read_byte(doc->file);
		}

		doc->binary_data=1;
		for(i=0; i<4; i++){
			c=pdf_read_byte(doc->file);
			if (  c < 128 ){
				doc->binary_data=0;
				break;
			}
		}
	}

	/**
	*	@fn					pdf_check_start_overheads(pdf_document *doc)
	*	@param[in]	doc		PDF Document to be filled
	*	@brief		In our case the PDF files are downloaded from Internet
	*	by CURL but while saving the document it also save some unnecessary
	*	white spaces. Due to these bytes the offset provided in PDF document
	*	doesn't reach at desired location and require to add white spaces.
	**/
	void pdf_start_overheads(pdf_document *doc){
		int i=0;

		pdf_seek(doc->file, 0, PDFSEEK_SET);

		while (pdf_iswhite(pdf_peek_byte(doc->file))){
			pdf_read_byte(doc->file);
			i++;
		}

		doc->overhead=i;
	}


	/**
	*	@fn					pdf_file_version(pdf_document *doc)
	*	@param[in]	doc		PDF Document to be filled
	*	@brief		Reads the PDF File version from the PDF File.
	**/
	void pdf_read_version(pdf_document *doc){
		//Before Reading file version sometimes it happens that the file
		//doesn't start with  required %%PDF tag thus he have to check
		//the starting overheads
		pdf_start_overheads(doc);

		char buf[20];

		pdf_read_line(doc->file, buf, sizeof buf);
		if (memcmp(buf, "%PDF-", 5) != 0){
			printf("cannot recognize version marker\n");
			exit(0);
		}

		doc->version = (int)(10 * (pdf_atof(buf+5) + 0.05));

		pdf_contains_binary(doc);
	}


	/**
	*	@fn			pdf_load_xref(pdf_document *doc, pdf_lexbuf *buf)
	*	@param[in]	doc		PDF Document Structure for reading PDF File
	*	@param[in]	buf		PDF Lexical Conventions Structure
	*	@brief		Function is used for loading the document provided. It will
	*	check each object of PDF Document and extract the text from PDF Document
	**/
	void pdf_load_xref(pdf_document *doc, pdf_lexbuf *buf PDFUnused){
		//First we have to Read the version of PDF File with the version number
		//some of the features of PDF File may to turned on or off.
		pdf_read_version(doc);

		//read tag startxref
		pdf_load_startxref(doc);

		//Now we can load the xref section
		pdf_load_xref_last(doc, doc->startxref, buf, 1);
	}

	C_MODE_END
