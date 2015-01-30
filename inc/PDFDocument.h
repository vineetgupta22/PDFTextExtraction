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

	#ifndef		_PDFTextDocument_
		/**
		*	@def		_PDFTextDocument_
		*	@brief		Header Including Parameter for Only Once.
		**/
		#define			_PDFTextDocument_

		//doing alignment of 4 bytes as required by some systems
		#pragma		pack(push, 4)


		typedef		struct PDFTextExtraction_Document_Structure				pdf_document;
		typedef 	struct PDFTextExtraction_Documents_Object 				pdf_obj;
		typedef 	struct PDFTextExtraction_Documents_Xref_Sections 		pdf_xref;
		typedef 	struct PDFTextExtraction_Documents_Xref_SubSections		pdf_xref_subsec;
		typedef 	struct PDFTextExtraction_Documents_Offsets_list			offsets_list;


		/**
		*	@struct		:		PDFTextExtraction_Documents_Xref_Sections
		*	@brief		:		Structure to store xref sections
		**/
		struct PDFTextExtraction_Documents_Xref_Sections{
			/**
			*	@var		:			objects
			*	@brief		:			Total Number of Object found in PDF
			**/
			int							objects;

			/**
			*	@var		:			subsec
			*	@brief		:			For Xref Sub Section
			**/
			pdf_xref_subsec				*subsec;

			/**
			*	@var		:			trailer
			*	@brief		:			For trailer written below xref section
			**/
			pdf_trailer					*trailer;
		};

		/**
		*	@struct		:		PDFTextExtraction_Documents_Offsets_list
		*	@brief		:		List of offsets found in Documents
		**/
		struct PDFTextExtraction_Documents_Offsets_list{
			int					max;
			int					len;
			int					*list;
		};

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

			/**
			*	@var		:			FileSize
			*	@brief		:			File Size of Input PDF File
			**/
			int							FileSize;

			/**
			*	@var		:			startxref
			*	@brief		:			offset of xref
			**/
			int							startxref;

			/**
			*	@var		:			xref_sections
			*	@brief		:			xref sections found in Document
			**/
			pdf_xref 					*xref_sections;

			/**
			*	@var		:			total_xref_sections
			*	@brief		:			Total xref section in PDF File
			**/
			int							total_xref_sections;
		};

		extern int errno;

		//Endling of alignment
		#pragma		pack(pop)
	#endif