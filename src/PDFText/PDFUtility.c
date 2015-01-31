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
	*	@param[in]	ch		Character is white space or not
	*	@result				Return the character is PDF White Space true or false
	*	@brief				Function is used Check whether the provided character
	*	is a white space character or not. White Space Characters are described in
	*	PDF Manual.
	**/
	int pdf_iswhite(int ch){
		return (ch == 0x00 || ch == 0x09 || ch == 0x0A ||
				ch == 0x0C || ch == 0x0D || ch == 0x20
		);
	}


	C_MODE_END
