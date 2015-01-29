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
*	@file		:	Main.c
*	@brief		:	Exe Project 
**/

	#include <PDFTextExtraction.h>

	int main(int argc PDFUnused, char **argv PDFUnused){

		//Whether you want to extract text from directory or
		//File. You just have to call one function

		//Calling the text extraction the File
		PDFTextExtraction("docs/judfile2.pdf");

		return 0;
	}
