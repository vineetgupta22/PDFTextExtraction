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
*	@file		:	PDFObjects.h
*	@brief		:	PDF Document Structure Objects
**/

	#ifndef		_PDFObjects_
		/**
		*	@def		_PDFObjects_
		*	@brief		Header Including Parameter for Only Once.
		**/
		#define			_PDFObjects_

		//doing alignment of 4 bytes as required by some systems
		#pragma		pack(push, 4)

		/**
		*	@enum			PDFTextExtraction_Documents_Object_Kind
		*	@brief			Various types of object described in PDF documentation
		**/
		typedef enum PDFTextExtraction_Documents_Object_Kind{
			/* NULL Object - as defined on Page 21 of 1.7 32000-1:2008*/
			PDF_NULL = 0,

			/* Boolean Object - as defined on Page 14 of 1.7 32000-1:2008*/
			PDF_BOOL = 'b',

			/* Numeric Object - Integer - as defined on Page 14 of 1.7 32000-1:2008*/
			PDF_INT = 'i',

			/* Numeric Object - Real - as defined on Page 14 of 1.7 32000-1:2008*/
			PDF_REAL = 'f',

			/* String Object - as defined on Page 14 of 1.7 32000-1:2008*/
			PDF_STRING = 's',

			/* String Object - as defined on Page 16 of 1.7 32000-1:2008*/
			PDF_NAME = 'n',

			/* Array Object - as defined on Page 18 of 1.7 32000-1:2008*/
			PDF_ARRAY = 'a',

			/* Dictionary Object - as defined on Page 18 of 1.7 32000-1:2008*/
			PDF_DICT = 'd',

			/* InDirect Object - as defined on Page 21 of 1.7 32000-1:2008*/
			PDF_INDIRECT = 'r'
		}pdf_object_kind;


		typedef 	struct PDFTextExtraction_Documents_trailer 				pdf_trailer;


		//Endling of alignment
		#pragma		pack(pop)
	#endif