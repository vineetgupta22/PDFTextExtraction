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
			*	@var		:			prev
			*	@brief		:			Pointer to Previous content stream
			**/
			pdf_contents 				*prev;

			/**
			*	@var		:			file
			*	@brief		:			Pointer to PDF File Stream
			**/
			pdf_stream 					*file;

			/**
			*	@var		:			resources
			*	@brief		:			Holding resources for Current Page
			**/
			pdf_obj						*resources;

			/**
			*	@var		:			doc
			*	@brief		:			Holding Document Details
			**/
			pdf_document				*doc;

			/**
			*	@var		:			PageNo
			*	@brief		:			Page Number of Current Processing Document
			**/
			int							PageNo;

			/**
			*	@var		:			next
			*	@brief		:			Pointer to Next content stream
			**/
			pdf_contents 				*next;
		};

		//Endling of alignment
		#pragma		pack(pop)
	#endif