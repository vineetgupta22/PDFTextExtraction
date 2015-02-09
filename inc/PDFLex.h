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
*	@file		:	PDFLex.h
*	@brief		:	For possing Lex
**/

	#ifndef		_PDFLex_
		/**
		*	@def		_PDFLex_
		*	@brief		Header Including Parameter for Only Once.
		**/
		#define			_PDFLex_

		//doing alignment of 4 bytes as required by some systems
		#pragma		pack(push, 4)

		#define 		RANGE_0_7 				'0':case'1':case'2':case'3':case'4':case'5':case'6':case'7'
		#define 		RANGE_0_9				RANGE_0_7:case'8':case'9'
		#define 		RANGE_a_f  				'a':case'b':case'c':case'd':case'e':case'f'
		#define 		RANGE_A_F 				'A':case'B':case'C':case'D':case'E':case'F'
		#define 		IS_NUMBER 				'+':case'-':case'.':case RANGE_0_9
		#define 		IS_HEX					RANGE_0_9 : case RANGE_a_f : case RANGE_A_F
		#define 		IS_WHITE				'\000':case'\011':case'\012':case'\014':case'\015':case'\040'
		#define 		IS_DELIM 				'(':case')':case'<':case'>':case'[':case']':case'{': case'}':case'/':case'%'
		#define			pdf_unread_byte(stm)	stm->rp--;

		enum{
			PDFTextExt_LEXBUF_SMALL = 256,
			PDFTextExt_LEXBUF_LARGE = 65536
		};


		typedef		struct PDFTextExtraction_LexBuffer_Structure					pdf_lexbuf;
		typedef		struct PDFTextExtraction_LexBuffer_Large_Structure				pdf_lexbuf_large;


		/**
		*	@struct		:		PDFTextExtraction_LexBuffer_Structure
		*	@brief		:		pdf_lexbuf Storage of last processed buffer type of lex etc
		**/
		struct PDFTextExtraction_LexBuffer_Structure{
			/**
			*	@var	:		size
			*	@brief	:		for keeping the size of buffer
			**/
			int 				size;

			/**
			*	@var	:		base_size
			*	@brief	:		for keeping the base size of buffer
			**/
			int 				base_size;

			/**
			*	@var	:		len
			*	@brief	:		for keeping the length of buffer
			**/
			int 				len;

			/**
			*	@var	:		i
			*	@brief	:		for keeping number of increments
			**/
			int 				i;

			/**
			*	@var	:		scratch
			*	@brief	:		for keeping buffer of lex element
			**/
			char 				*scratch;

			/**
			*	@var	:		f
			*	@brief	:		For storage of float value
			**/
			float				f;

			/**
			*	@var	:		buffer
			*	@brief	:		Extra buffer if required for storage
			**/
			char 				buffer[PDFTextExt_LEXBUF_SMALL];
		};


		/**
		*	@struct		:		PDFTextExtraction_LexBuffer_Large_Structure
		*	@brief		:		pdf_lexbuf_large is the structure for processing
		*	buffer as lex elements
		**/
		struct PDFTextExtraction_LexBuffer_Large_Structure{
			pdf_lexbuf 					base;

			/**
			*	@var	:		buffer
			*	@brief	:		Keeping the buffer for processing lex structure
			**/
			char buffer[PDFTextExt_LEXBUF_LARGE - PDFTextExt_LEXBUF_SMALL];
		};

		typedef enum{
			PDF_TOK_ERROR,
			PDF_TOK_EOF,
			PDF_TOK_OPEN_ARRAY,
			PDF_TOK_CLOSE_ARRAY,
			PDF_TOK_OPEN_DICT,
			PDF_TOK_CLOSE_DICT,
			PDF_TOK_OPEN_BRACE,
			PDF_TOK_CLOSE_BRACE,
			PDF_TOK_NAME,
			PDF_TOK_INT,
			PDF_TOK_REAL,
			PDF_TOK_STRING,
			PDF_TOK_KEYWORD,
			PDF_TOK_R,
			PDF_TOK_TRUE,
			PDF_TOK_FALSE,
			PDF_TOK_NULL,
			PDF_TOK_OBJ,
			PDF_TOK_ENDOBJ,
			PDF_TOK_STREAM,
			PDF_TOK_ENDSTREAM,
			PDF_TOK_XREF,
			PDF_TOK_TRAILER,
			PDF_TOK_STARTXREF,
			PDF_NUM_TOKENS
		} pdf_token;


		//Endling of alignment
		#pragma		pack(pop)
	#endif