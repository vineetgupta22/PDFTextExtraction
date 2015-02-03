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
		extern void pdf_free_stream(pdf_stream *stm);
		extern pdf_stream *pdf_open_file(const char *name);
		extern pdf_stream *pdf_keep_stream(pdf_stream *stm);
		extern void pdf_read_line(pdf_stream *stm, char *mem, int n);
		extern void pdf_seek(pdf_stream *stm, int offset, int whence);
		extern int pdf_read(pdf_stream *stm, unsigned char *buf, int len);

		//Functions defined in src/PDFTrailer.c
		int pdf_xref_size_from_trailer(pdf_document *doc, pdf_lexbuf *buf);

		//Function defined under src/PDFLex.c
		extern void pdf_lexbuf_init(pdf_lexbuf *lex_buffer, int size);
		extern pdf_token pdf_lex(pdf_stream *f, pdf_lexbuf *buf);


		//Functions defined under src/PDFUtility.c
		extern int pdf_iswhite(int ch);


		//Functions defined under src/PDFString.c
		extern int pdf_atoi(const char *s);
		extern float pdf_atof(const char *s);
		extern char *pdf_strsep(char **str, const char *delim);


		//Functions defined under src/PDFXref.c
		extern void pdf_load_xref(pdf_document *doc, pdf_lexbuf *buf);

		//Functions defined under src/PDFParser.c
		extern pdf_trailer *pdf_parse_dict(pdf_document *doc, pdf_stream *file, pdf_lexbuf *buf);
		extern pdf_obj *pdf_parse_ind_obj(pdf_document *doc, pdf_stream *file, pdf_lexbuf *buf, int *onum, int *ogen, int *ostmofs);

		//Functions defined under src/PDFObjects.c
		extern pdf_obj *pdf_new_int(int i);
		extern int pdf_to_int(pdf_obj *obj);
		extern pdf_obj *pdf_new_bool(int b);
		extern pdf_obj *pdf_new_dict(void);
		extern char *pdf_to_name(pdf_obj *obj);
		extern void pdf_drop_obj(pdf_obj *obj);
		extern int pdf_array_len(pdf_obj *obj);
		extern pdf_obj *pdf_keep_obj(pdf_obj *obj);
		extern void pdf_free_trailer(pdf_obj *obj);
		extern pdf_obj *pdf_new_name(const char *str);
		extern pdf_obj *pdf_new_indirect(int num, int gen);
		extern int pdf_mark_obj(pdf_document *doc, pdf_obj *obj);
		extern pdf_obj *pdf_new_string(const char *str, int len);
		extern void pdf_unmark_obj(pdf_document *doc, pdf_obj *obj);
		extern pdf_obj * pdf_new_array(pdf_document *doc, int initialcap);
		extern void pdf_array_push(pdf_document *doc, pdf_obj *obj, pdf_obj *item);
		extern pdf_obj *pdf_dict_gets(pdf_document *doc, pdf_obj *obj, const char *key);
		extern void pdf_dict_put(pdf_document *doc, pdf_obj *obj, pdf_obj *key, pdf_obj *val);

		//Function defined in src/PDFIndirect.c
		extern pdf_obj *pdf_resolve_indirect(pdf_document *doc, pdf_obj *ref);
		extern pdf_obj *pdf_array_get(pdf_obj *obj, int i);

		//Function defined in src/PDFPages.c
		extern void pdf_load_page(pdf_document *doc, int number);


		/*********** Starting of Memory Allocation [File = PDFMem.c]***********/
		//linking to Memory Allocation
		extern PDFExport void * PDFAPI PDFMemAlloc(unsigned int size, const char *file, unsigned int line);

		//linking to Free Memory Allocation
		extern PDFExport void PDFAPI PDFMemFree (void *Address, const char *file, int line);

		//Linking to print Memory Details
		extern PDFExport void PDFAPI PDFMemDetails(void);

		extern PDFExport void * PDFAPI PDFMemReAlloc(void *Address, unsigned int size, const char *file, unsigned int line);
		/****************** Ending of of Memory Allocation ********************/


		//Endling of alignment
		#pragma		pack(pop)
	#endif