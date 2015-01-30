/**
*	Copyright (c) 2008-2015, Cee Emm Infotech and/or its affiliates.
*	All rights are reserved.
*
*	Program Name:		PDFTextExtraction
*	Program Owner:		Cee Emm Infotech
*	Chief Developer:	Vineet Gupta
*
*	contact: vineetgupta22@gmail.com
*	Address: Cee Emm Infotech, 605, Sector 10D, Chandgiarh [INDIA]
*
**/


/**
*	@file		:	PDFUtility.c
*	@brief		:	Some Utility functions used in PDF
**/

	#include <PDFTextExtraction.h>

	C_MODE_START


	/***************************** Starting Prototypes ********************/
	int pdf_iswhite(int ch);
	/***************************** Ending Prototypes **********************/

	/***************************** Global Variables ********************/
	/***************************** Global Variables ********************/


	/**
	*	@fn					pdf_iswhite(int ch)
	*	@param[in]	ch		Name of the File
	*	@result			Return the character is PDF White Space true or false
	*	@brief		Checking the provided char is white space according to the
	*	documentation of PDF.
	**/
	int pdf_iswhite(int ch){
		return 	ch == '\000' || ch == '\011' ||
				ch == '\012' || ch == '\014' ||
				ch == '\015' || ch == '\040';
	}


	C_MODE_END
