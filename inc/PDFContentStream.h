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

		typedef 	struct PDFTextExtraction_Documents_Content_Stream					pdf_contents;
		typedef 	struct PDFTextExtraction_Documents_Content_Stream_Stack				pdf_contents_stack;


		struct PDFTextExtraction_Documents_Content_Stream_Stack{
			/**
			*	@var		:			obj
			*	@brief		:			While parsing text, needed to store any object
			**/
			pdf_obj 					*obj;

			/**
			*	@var		:			name
			*	@brief		:			While parsing text for storing name token
			**/
			char 						name[256];

			/**
			*	@var		:			string
			*	@brief		:			While parsing text for storing string
			**/
			unsigned char 				string[256];

			/**
			*	@var		:			string_len
			*	@brief		:			While parsing text for storing string length used
			**/
			int 						string_len;

			/**
			*	@var		:			stack
			*	@brief		:			While parsing text for Numbers in stack
			**/
			float 						stack[32];

			/**
			*	@var		:			top
			*	@brief		:			Stack tracking of filled
			**/
			int 						top;

			/**
			*	@var		:			xbalance
			*	@brief		:			For tracking balance of tex
			**/
			int 						xbalance;

			/**
			*	@var		:			in_text
			*	@brief		:			Keeping track of Text Block begins
			**/
			int 						in_text;
		};

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
			*	@var		:			stack
			*	@brief		:			Used for holding temporary values
			**/
			pdf_contents_stack			*stack;

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