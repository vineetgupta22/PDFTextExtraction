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
*	@file		:	PDFStream.h
*	@brief		:	Including Structures of Streams
**/

	#ifndef		_PDFStream_
		/**
		*	@def	:	_PDFStream_
		*	@brief	:	Header Including Parameter for Only Once.
		**/
		#define			_PDFStream_

		//doing alignment of 4 bytes as required by some systems
		#pragma		pack(push, 4)


		typedef		struct PDFTextExtraction_Stream_Structure				pdf_stream;
		typedef		struct PDFTextExtraction_File_Stream_Structure			pdf_file_stream;


		/**
		*	@struct		:		PDFTextExtraction_File_Stream_Structure
		*	@brief		:		pdf_file_stream is the structure actually holding the
		*	opened file and buffer having enough space to read/write to file
		**/
		struct PDFTextExtraction_File_Stream_Structure{
			/**
			*	@var	:		file
			*	@brief	:		File Descriptor holding file
			**/
			int 				file;

			/**
			*	@var	:		buffer
			*	@brief	:		I/O Buffer
			**/
			unsigned char 		buffer[4096];
		};


		/**
		*	@struct		:		PDFTextExtraction_Stream_Structure
		*	@brief		:		pdf_stream is a buffered reader capable of seeking
		*	in both directions. Streams are reference counted, so references must be
		*	dropped by a call to pdf_close. Only the data between rp and wp is valid.
		**/
		struct PDFTextExtraction_Stream_Structure{
			int 						refs;
			int 						error;
			int 						eof;
			int 						pos;
			int 						avail;
			int 						bits;
			unsigned char 				*rp;
			unsigned char				*wp;
			void 						*state;
		};

		//Endling of alignment
		#pragma		pack(pop)
	#endif