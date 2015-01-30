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
*	@file		:	PDFAll.h
*	@brief		:	Including Object Structures etc.
**/

	#ifndef		_PDFAll_
		/**
		*	@def		_PDFAll_
		*	@brief		Header Including Parameter for Only Once.
		**/
		#define			_PDFAll_

		//doing alignment of 4 bytes as required by some systems
		#pragma		pack(push, 4)

		extern 		unsigned long 				MallocSize;

		//Including Stream structures
		#ifndef		_PDFStream_
			#include <PDFStream.h>
		#endif

		//Including Lex Structures
		#ifndef		_PDFLex_
			#include <PDFLex.h>
		#endif

		//Including Lex Structures
		#ifndef		_PDFObjects_
			#include <PDFObjects.h>
		#endif

		//Including document structures
		#ifndef		_PDFDocument_
			#include <PDFDocument.h>
		#endif

		//Endling of alignment
		#pragma		pack(pop)
	#endif