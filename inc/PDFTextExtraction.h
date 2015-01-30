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


	/**
	* 	@mainpage 			PDF Text Extraction
	*
	*	@section	Intro	Introduction
	*						The Project  is  the Internal Part of Project other
	*	Major  Product. The  program  is  used  for extraction of Text from PDF
	*	Files. Since there are many Libraries available in the Open Source  but
	*	none is describing or doing the extraction as LINE by LINE. Since,  the
	*	extraction  of  text  LINE by LINE is not available in Open Source thus
	*	created project to others to do things according to there needs.
	*
	*	Cross Platform
	*	==============
	*	We are now creating binaries to support three main OS i.e.Windows, Linux
	*	and Mac. We have created this project on CYGWIN with the help of GNU GCC
	*	Compiler using  AutoTool  Kit to support the cross platform development.
	*	For  windows  separate DLL file is created and for Linux and Mac Shared
	*	Library is created.
	*
	*	Compiling
	*	=========
	*	As we are using AutoTools thus a file name bootstrap created to configure
	*	the project.  As  other only provide configure file and hide configure.ac
	*	file but we have included the configure.ac to describe or to tell how to
	*	make your project easily cross platform completable.
	*
	* 	@author 	Vineet Gupta, 	vineetgupta22@gmail.com
	*	Address: Cee Emm Infotech, 605, Sector 10D, Chandgiarh [INDIA]
	**/


	#ifndef		__PDFTextExtraction__
		/**
		*	@def		__PDFTextExtraction__
		*	@brief		Header Including Parameter for Only Once.
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

		/**
		*	standard symbolic constants and types
		**/
		#ifdef HAVE_UNISTD_H
			#include<unistd.h>
		#endif

		//Defined Macros
		#ifndef _PDFTextExtractionMacros_
			#include<PDFMacros.h>
		#endif

		//Including Structures typedef etc
		#ifndef _PDFTextExtractionAll_
			#include<PDFAll.h>
		#endif

		//Defined extern Functions
		#ifndef _PDFTextExtractionExtern_
			#include<PDFExtern.h>
		#endif

		//Endling of alignment
		#pragma		pack(pop)
	#endif