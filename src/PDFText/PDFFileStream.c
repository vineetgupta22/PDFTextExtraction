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
*	@file		:	PDFFileStream.c
*	@brief		:	Functions Realting to Streams
**/

	#include <PDFTextExtraction.h>

	C_MODE_START


	/***************************** Starting Prototypes ********************/
	pdf_stream *pdf_fd(int fd);
	pdf_stream *pdf_new_stream(void *state);
	pdf_stream *pdf_open_file(const char *name);
	pdf_stream *pdf_keep_stream(pdf_stream *stm);
	/***************************** Ending Prototypes **********************/

	/***************************** Global Variables ********************/
	/***************************** Global Variables ********************/


	pdf_stream *pdf_open_file(const char *name){
		int fd;

		//Opening the File in Binary ReadOnly Mode
		fd = open(name, O_BINARY | O_RDONLY, 0);

		return pdf_fd(fd);
	}


	pdf_stream *pdf_new_stream(void *state){
		pdf_stream *stm;

		stm=(pdf_stream*)malloc(sizeof(pdf_stream));
		memset(stm, 0, sizeof(pdf_stream));

		//Increasing reference counts
		stm->refs = 1;
		stm->state = state;

		//Returning stream
		return stm;
	}


	pdf_stream *pdf_fd(int fd){
		pdf_file_stream *state;

		//Memory Allocation of stream file
		state=(pdf_file_stream*)malloc(sizeof(pdf_file_stream));
		memset(state, 0, sizeof(pdf_file_stream));

		state->file = fd;

		return pdf_new_stream(state);
	}


	pdf_stream *pdf_keep_stream(pdf_stream *stm){
		if (stm)
			stm->refs ++;
		return stm;
	}


	C_MODE_END
