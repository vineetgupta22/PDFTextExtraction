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


		//Endling of alignment
		#pragma		pack(pop)
	#endif