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
*	@file		:	PDFTextExtraction.h
*	@brief		:	Header File
**/

	#ifndef		__PDFTextExtraction__
		/**
		*	@def	:	__PDFTextExtraction__
		*	@brief	:	Header Including Parameter for Only Once.
		**/
		#define			__PDFTextExtraction__

		//doing alignment of 4 bytes as required by some systems
		#pragma		pack(push, 4)
		
		/**
		*	Including Configuration file generated by Compiler
		**/
		#ifdef		HAVE_CONFIG_H
			#include <config.h>
		#endif

		/**
		*	Standard Input/Output Libarary
		**/
		#ifdef	HAVE_STDIO_H
			#include <stdio.h>
		#endif

		/**
		*	Including Standard Libaray
		**/
		#ifdef	HAVE_STDLIB_H
			#include <stdlib.h>
		#endif

		/**
		*	Including Standard Strings Libaray
		**/
		#ifdef	HAVE_STRINGS_H
			#include <string.h>
		#endif

		/**
		*	Including System types
		**/
		#ifdef HAVE_SYS_TYPES_H
			#include<sys/types.h>
		#endif

		/**
		*	Including for Reading Directories
		**/
		#ifdef HAVE_DIRENT_H
			#include<dirent.h>
		#endif

		/**
		*	Low Level Input/Output Function
		**/
		#ifdef HAVE_FCNTL_H
			#include<fcntl.h>
		#endif

		//Defined Macros
		#ifndef _PDFTextExtractionMacros_
			#include<PDFMacros.h>
		#endif

		//Defined extern Functions
		#ifndef _PDFTextExtractionExtern_
			#include<PDFExtern.h>
		#endif

		//Endling of alignment
		#pragma		pack(pop)
	#endif