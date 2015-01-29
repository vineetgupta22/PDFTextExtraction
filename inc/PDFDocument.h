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
*	@file		:	PDFDocument.h
*	@brief		:	PDF Document Structure
**/

	#ifndef		_PDFDocument_
		/**
		*	@def	:	_PDFDocument_
		*	@brief	:	Header Including Parameter for Only Once.
		**/
		#define			_PDFDocument_

		//doing alignment of 4 bytes as required by some systems
		#pragma		pack(push, 4)


		typedef		struct PDFTextExtraction_Document_Structure				pdf_document;


		/**
		*	@struct		:		PDFTextExtraction_Document_Structure
		*	@brief		:		pdf_document is the structure actually holding
		*	all the structures, file, xref, object and all other things
		**/
		struct PDFTextExtraction_Document_Structure{
			/**
			*	@var		:			file
			*	@brief		:			Pointer to PDF File Stream
			**/
			pdf_stream 					*file;

			/**
			*	@var		:			lexbuf
			*	@brief		:			used for token and lex elements
			**/
			pdf_lexbuf_large			lexbuf;

			/**
			*	@var		:			overhead
			*	@brief		:			Overhead bytes in File starting
			**/
			int							overhead;

			/**
			*	@var		:			version
			*	@brief		:			Version of PDF File
			**/
			int							version;
		};

		extern int errno;

		//Endling of alignment
		#pragma		pack(pop)
	#endif