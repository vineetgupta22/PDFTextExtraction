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
	int pdf_tell(pdf_stream *stm);
	int pdf_read_byte(pdf_stream *stm);
	int pdf_peek_byte(pdf_stream *stm);
	int pdf_next(pdf_stream *stm, int n);
	pdf_stream *pdf_new_stream(void *state);
	pdf_stream *pdf_open_file(const char *name);
	pdf_stream *pdf_keep_stream(pdf_stream *stm);
	void pdf_read_line(pdf_stream *stm, char *mem, int n);
	void pdf_seek_file(pdf_stream *stm, int offset, int whence);
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
		MallocSize+=sizeof(pdf_stream);

		//Increasing reference counts
		stm->refs = 1;
		stm->state = state;

		stm->seek = pdf_seek_file;
		stm->next = pdf_next;

		//Returning stream
		return stm;
	}


	pdf_stream *pdf_fd(int fd){
		pdf_file_stream *state;

		//Memory Allocation of stream file
		state=(pdf_file_stream*)malloc(sizeof(pdf_file_stream));
		memset(state, 0, sizeof(pdf_file_stream));
		MallocSize+=sizeof(pdf_file_stream);

		state->file = fd;

		return pdf_new_stream(state);
	}


	pdf_stream *pdf_keep_stream(pdf_stream *stm){
		if (stm)
			stm->refs ++;
		return stm;
	}


	void pdf_seek_file(pdf_stream *stm, int offset, int whence){
		pdf_file_stream *state = stm->state;

		long long n = PDFSeek(state->file, offset, whence);

		if (n < 0){
			printf("cannot seek in file\n");
			exit(0);
		}

		stm->pos = (int)n;
		stm->rp = state->buffer;
		stm->wp = state->buffer;
	}


	int pdf_tell(pdf_stream *stm){
		return stm->pos - (stm->wp - stm->rp);
	}


	int pdf_peek_byte(pdf_stream *stm){
		int c;

		if (stm->rp != stm->wp){
			return *stm->rp;
		}

		c = stm->next(stm, 1);

		if (c != EOF){
			stm->rp--;
		}

		return c;
	}


	int pdf_read_byte(pdf_stream *stm){
		int c = EOF;

		if (stm->rp != stm->wp)
			return *stm->rp++;

		c = stm->next(stm, 1);

		if (c == EOF){
			stm->eof = 1;
		}

		return c;
	}


	int pdf_next(pdf_stream *stm, int n){
		pdf_file_stream *state = stm->state;

		/* n is only a hint, that we can safely ignore */
		n = PDFRead(state->file, state->buffer, sizeof(state->buffer));

		if (n < 0){
			printf("error in reading file contents\n");
			exit(0);
		}

		stm->rp = state->buffer;
		stm->wp = state->buffer + n;
		stm->pos += n;

		if (n == 0){
			return EOF;
		}

		return *stm->rp++;
	}


	void pdf_seek(pdf_stream *stm, int offset, int whence){
		/* Reset bit reading */
		stm->avail = 0;

		if (stm->seek){
			if (whence == 1){
				offset = pdf_tell(stm) + offset;
				whence = 0;
			}
			stm->seek(stm, offset, whence);
			stm->eof = 0;
		}else if (whence != 2){
			if (whence == 0){
				offset -= pdf_tell(stm);
			}
			if ( offset < 0 ){
				printf("cannot seek backwards\n");
				exit(0);
			}
			/* slow dog , but rare enough */
			while (offset-- > 0){
				if (pdf_read_byte(stm) == EOF){
					printf("seek failed\n");
					exit(0);
				}
			}
		}else{
			printf("cannot seek in file\n");
			exit(0);
		}
	}


	void pdf_read_line(pdf_stream *stm, char *mem, int n){
		char *s = mem;
		int c = EOF;
		while (n > 1){
			c = pdf_read_byte(stm);
			if (c == EOF)
				break;
			if (c == '\r') {
				c = pdf_peek_byte(stm);
				if (c == '\n')
					pdf_read_byte(stm);
				break;
			}
			if (c == '\n')
				break;
			*s++ =(char)c;
			n--;
		}
		if (n)
			*s = '\0';
	}


	C_MODE_END
