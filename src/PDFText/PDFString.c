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
*	@file		:	PDFString.c
*	@brief		:	Functions relating to strings
**/

	#include <PDFTextExtraction.h>
	#include <errno.h>			//for errno
	#include <math.h>			//for isnan
	#include <float.h>			//for FLT_MAX

	C_MODE_START


	/***************************** Starting Prototypes ********************/
	float pdf_atof(const char *s);
	char *pdf_strsep(char **str, const char *delim);
	double pdf_clampd(double d, double min, double max);
	int pdf_strlcpy(char *dst, const char *src, int siz);
	/***************************** Ending Prototypes **********************/

	/***************************** Global Variables ********************/
	/***************************** Global Variables ********************/

	int pdf_strlcpy(char *dst, const char *src, int siz){
		register char *d = dst;
		register const char *s = src;
		register int n = siz;

		/* Copy as many bytes as will fit */
		if (n != 0 && --n != 0) {
			do {
				if ((*d++ = *s++) == 0)
					break;
			} while (--n != 0);
		}

		/* Not enough room in dst, add NUL and traverse rest of src */
		if (n == 0) {
			if (siz != 0)
				*d = '\0';		/* NUL-terminate dst */
			while (*s++)
				;
		}

		/* count does not include NUL */
		return(s - src - 1);
	}

	double pdf_clampd(double d, double min, double max){
		return (d > min ? (d < max ? d : max) : min);
	}


	float pdf_atof(const char *s){
		double d;

		errno = 0;

		d = strtod(s, NULL);

		if (errno == ERANGE || isnan(d)) {
			return 1.0;
		}

		d = pdf_clampd(d, -FLT_MAX, FLT_MAX);
		return (float)d;
	}


	char *pdf_strsep(char **str, const char *delim){
		char *ret = *str;

		if (!ret){
			return NULL;
		}

		if ((*str = strpbrk(*str, delim)) != NULL){
			*((*str)++) = '\0';
		}

		return ret;
	}

	int pdf_atoi(const char *s){
		return atoi(s);
	}

	C_MODE_END
