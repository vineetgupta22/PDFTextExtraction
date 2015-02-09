/**
*	@file		:	PDFContentStream.h
*	@brief		:	Contents Stream used in Project
**/

	#ifndef		_PDFContentStream_
		/**
		*	@def		_PDFContentStream_
		*	@brief		Header Including Parameter for Only Once.
		**/
		#define			_PDFContentStream_

		//doing alignment of 4 bytes as required by some systems
		#pragma		pack(push, 4)

		typedef 	struct PDFTextExtraction_Documents_Content_Stream			pdf_contents;

		struct PDFTextExtraction_Documents_Content_Stream{
			/**
			*	@var		:			file
			*	@brief		:			Pointer to PDF File Stream
			**/
			pdf_stream 					*file;
		};

		//Endling of alignment
		#pragma		pack(pop)
	#endif