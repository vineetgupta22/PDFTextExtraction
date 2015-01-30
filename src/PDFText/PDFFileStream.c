/**
*	@file			PDFFileStream.c
*	@brief			Functions Relating to Streams
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
	int pdf_available(pdf_stream *stm, int max);
	pdf_stream *pdf_open_file(const char *name);
	pdf_stream *pdf_keep_stream(pdf_stream *stm);
	void pdf_read_line(pdf_stream *stm, char *mem, int n);
	int pdf_read(pdf_stream *stm, unsigned char *buf, int len);
	void pdf_seek_file(pdf_stream *stm, int offset, int whence);
	/***************************** Ending Prototypes **********************/

	/***************************** Global Variables ********************/
	/***************************** Global Variables ********************/


	/**
	*	@fn			pdf_stream *pdf_open_file(const char *name)
	*	@param[in]	name	Name of the File
	*	@result		Return the pdf_stream structure for doing input and output
	*	on File Provided.
	*	@brief		The Function opens the Input file as binary read only mode
	*	and return the structure of pdf_stream used for read stream and process
	*	file according to references provided in the file.
	**/
	pdf_stream *pdf_open_file(const char *name){
		int fd;

		//Opening the File in Binary ReadOnly Mode
		fd = open(name, O_BINARY | O_RDONLY, 0);

		return pdf_fd(fd);
	}


	/**
	*	@fn			pdf_stream *pdf_new_stream(void *state)
	*	@param[in]	state	State of File
	*	@result		Return the pdf_stream structure for doing IO
	*
	*	@brief		Function creates the new pdf_stream on provided state.
	*	It is important function and it links the other functions such as
	*	seek, next etc. with pdf stream structure.
	**/
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


	/**
	*	@fn			pdf_stream *pdf_fd(int fd)
	*	@param[in]	fd	File Descriptor
	*	@result		Return the pdf_stream structure for doing IO
	*
	*	@brief		Function received the file descriptor and assign it to the
	*	state structure. The stream will take ownership of the file descriptor,
	*	so it may not be modified or closed after the call to pdf_fd. When the
	*	stream is closed it will also close the file descriptor.
	**/
	pdf_stream *pdf_fd(int fd){
		pdf_file_stream *state;

		//Memory Allocation of stream file
		state=(pdf_file_stream*)malloc(sizeof(pdf_file_stream));
		memset(state, 0, sizeof(pdf_file_stream));
		MallocSize+=sizeof(pdf_file_stream);

		state->file = fd;

		return pdf_new_stream(state);
	}


	/**
	*	@fn			pdf_stream *pdf_keep_stream(pdf_stream *stm)
	*	@param[in]	stm		PDF stream
	*	@result		After keeping pdf_stream it return kept stream
	*	@brief		Required to make sure the file descriptor not to
	*	be closed.
	**/
	pdf_stream *pdf_keep_stream(pdf_stream *stm){
		if (stm)
			stm->refs ++;
		return stm;
	}


	/**
	*	@fn						pdf_seek_file(pdf_stream *stm, int offset, int whence)
	*	@param	[in]	stm		PDF stream to read from
	*	@param	[in]	offset	The offset to seek to
	*	@param	[in]	whence	From where the offset is measured
	*	@brief					It does the real seeking on the file stream
	**/
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


	/**
	*	@fn						pdf_tell(pdf_stream *stm)
	*	@param	[in]	stm		PDF stream to read from
	*	@return					Position
	*	@brief					The function return the current reading
	*	position within a stream
	**/
	int pdf_tell(pdf_stream *stm){
		return stm->pos - (stm->wp - stm->rp);
	}


	/**
	*	@fn						pdf_peek_byte(pdf_stream *stm)
	*	@param	[in]	stm		PDF stream to read from
	*	@brief			Function Read the current byte from the stm and reset
	*	position to earlier position of the stream.
	**/
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


	/**
	*	@fn						pdf_read_byte(pdf_stream *stm)
	*	@param	[in]	stm		PDF stream to read from
	*	@brief			Function Read the current byte from the stm
	**/
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


	/**
	*	@fn						pdf_seek(pdf_stream *stm, int offset, int whence)
	*	@param	[in]	stm		PDF stream to read from
	*	@param	[in]	offset	The offset to seek to
	*	@param	[in]	whence	From where the offset is measured
	*	@brief					It sets the cursor in file to the desired location.
	*	For more information see fseek
	**/
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


	/**
	*	@fn						pdf_read_line(pdf_stream *stm, char *mem, int n)
	*	@param	[in]	stm		PDF stream to read from
	*	@param	[in]	mem		The data block to read into
	*	@param	[in]	n		The length of the data block (in bytes)
	*	@brief					Attempt to read a stream into a buffer.  If
	*	truncated is NULL. Otherwise does not throw exceptions in the case
	*	of failure, but instead sets a EOF flag.
	**/
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


	/**
	*	@fn						pdf_available(pdf_stream *stm, int max)
	*	@param	[in]	stm		PDF stream holding file descriptors
	*	@param	[in]	max		Number of bytes avaiable to read
	*	@brief					Returns the number of bytes immediately available
	*	between the read and write pointers. This number is guaranteed only to be 0
	*	if we have hit EOF. The number of bytes returned here need have
	*	no relation to max (could be larger, could be smaller).
	**/
	int pdf_available(pdf_stream *stm, int max){
		//available length
		int len = stm->wp - stm->rp;
		int c = EOF;

		if (len){
			return len;
		}

		//Loading the stream
		c = stm->next(stm, max);

		if (c == EOF){
			stm->eof=1;
			return 0;
		}

		stm->rp--;

		//Bytes available
		return stm->wp - stm->rp;
	}



	/**
	*	@fn						pdf_read(pdf_stream *stm, unsigned char *buf, int len)
	*	@param	[in]	stm		PDF stream to read from
	*	@param	[in]	buf		The data block to read into
	*	@param	[in]	len		The length of the data block (in bytes)
	*	@return					Returns the number of bytes read.
	*	@brief					Returns the number of bytes immediately available
	*	between the read and write pointers. This number is guaranteed only to be 0
	*	if we have hit EOF. The number of bytes returned here need have no
	*	relation to max (could not be larger, could be smaller).
	**/
	int pdf_read(pdf_stream *stm, unsigned char *buf, int len){
		int count, n;

		count = 0;

		//loop for reading bytes
		do{
			n = pdf_available(stm, len);
			if (n > len){
				//decreasing the number of bytes required
				n = len;
			}

			//No bytes are available
			if (n == 0){
				break;
			}

			//copy the buffer from input to output
			memcpy(buf, stm->rp, (size_t)n);

			//increasing the length count buffer
			stm->rp += n;
			buf += n;
			count += n;
			len -= n;
		}while (len > 0);

		return count;
	}

	C_MODE_END
