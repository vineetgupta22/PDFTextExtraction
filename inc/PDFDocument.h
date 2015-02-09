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
		typedef		struct PDFTextExtraction_Document_Buffer				pdf_buffer;
		typedef 	struct PDFTextExtraction_Documents_Xref_Sections 		pdf_xref;
		typedef 	struct PDFTextExtraction_Documents_Xref_Entry			pdf_xref_entry;
		typedef 	struct PDFTextExtraction_Documents_Xref_SubSections		pdf_xref_subsec;
		typedef 	struct PDFTextExtraction_Documents_Offsets_list			offsets_list;
		typedef 	struct PDFTextExtraction_Documents_PageLabels			pdf_pageslables;


		struct PDFTextExtraction_Documents_PageLabels{
			char				*kind;
			int					startfrom;
			pdf_pageslables		*prev;
			pdf_pageslables		*next;
		};

		/**
		*	@struct		:		PDFTextExtraction_Documents_Xref_SubSections
		*	@brief		:		Structure to store xref sub sections entry
		**/
		struct PDFTextExtraction_Documents_Xref_Entry{
			/**
			*	@var		:			type
			*	@brief		:			0=unset (f)ree i(n)use (o)bjstm
			**/
			char 						type;

			/**
			*	@var		:			flags
			*	@brief		:			bit 0 = marked
			**/
			unsigned char 				flags;

			/**
			*	@var		:			gen
			*	@brief		:			for generation/objstm index
			**/
			unsigned short 				gen;

			/**
			*	@var		:			offsets
			*	@brief		:			file offset / objstm object number
			**/
			int 						offsets;

			/**
			*	@var		:			stream_offset
			*	@brief		:			on-disk stream
			**/
			int 						stream_offset;
			/**
			*	@var		:			stm_buf
			*	@brief		:			in-memory stream (for updated objects)
			**/
			pdf_buffer 					*stm_buf;

			/**
			*	@var		:			obj
			*	@brief		:			stored/cached object
			**/
			pdf_obj 					*obj;

		};


		/**
		*	@struct		:		PDFTextExtraction_Documents_Xref_SubSections
		*	@brief		:		Structure to store xref sub sections
		**/
		struct PDFTextExtraction_Documents_Xref_SubSections{
			/**
			*	@var		:			start
			*	@brief		:			Starting Number of xref subsection
			**/
			int							start;

			/**
			*	@var		:			len
			*	@brief		:			Total number of entries in xref entry table
			**/
			int							len;

			/**
			*	@var		:			table
			*	@brief		:			Entries of xref entry
			**/
			pdf_xref_entry 				*table;

			/**
			*	@var		:			next
			*	@brief		:			Pointer to next xref sub section
			**/
			pdf_xref_subsec 			*next;
		};

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
			*	@var		:			binary_data
			*	@brief		:			PDF File Contains binary data or not
			**/
			int							binary_data;

			/**
			*	@var		:			startxref
			*	@brief		:			offset of xref
			**/
			int							startxref;

			/**
			*	@var		:			max_xref_len
			*	@brief		:			Maximum length of xref section
			**/
			int 						max_xref_len;

			/**
			*	@var		:			xref_index
			*	@brief		:			xref index found in Document
			**/
			int 						*xref_index;

			int 						freeze_updates;

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

			/**
			*	@var		:			page_count
			*	@brief		:			Total Number of Pages found in PDF File
			**/
			int 						page_count;

			/**
			*	@var		:			font
			*	@brief		:			Document Fond Details
			**/
			pdf_font					*font;

			/**
			*	@var		:			pageslabels
			*	@brief		:			Document Page Labels Details
			**/
			pdf_pageslables 			*pageslabels;
		};

		extern int errno;

		//Endling of alignment
		#pragma		pack(pop)
	#endif