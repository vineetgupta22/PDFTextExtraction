/**
*	@file		:	PDFTrailer.c
*	@brief		:	Reading blocks of trailer
**/

	#include <PDFTextExtraction.h>

	C_MODE_START


	/***************************** Starting Prototypes ********************/
	int pdf_xref_size_from_trailer(pdf_document *doc, pdf_lexbuf *buf);
	/***************************** Ending Prototypes **********************/

	/***************************** Global Variables ********************/
	/***************************** Global Variables ********************/

	int pdf_xref_size_from_trailer(pdf_document *doc, pdf_lexbuf *buf PDFUnused){
		int size=0, c, len, t;
		int offsets;
		char *s;

		//Recording the current position
		offsets = pdf_tell(doc->file);

		pdf_read_line(doc->file, buf->scratch, buf->size);

		//Checking the line must be xref
		if (strncmp(buf->scratch, "xref", 4) != 0){
			printf("cannot find the xref tag\n");
			exit(0);
		}

		while(1){
			//peek a byte
			c = pdf_peek_byte(doc->file);

			//xref sub section must start with 0 to 9
			if (!(c >= '0' && c <= '9')){
				break;
			}

			//reading line
			pdf_read_line(doc->file, buf->scratch, buf->size);

			s = buf->scratch;
			pdf_strsep(&s, " "); 				/* ignore ofs */
			len=pdf_atoi(pdf_strsep(&s, " "));	//Length of xref section

			if (len < 0){
				printf("xref range marker must be positive\n");
				exit(0);
			}

			t=pdf_tell(doc->file);
			if (len > (INT_MAX - t) / 20){
				printf("xref has too many entries can't handle all\n");
				exit(0);
			}

			//Jumping xref section according to length
			pdf_seek(doc->file, t + (20 * len), PDFSEEK_SET);
		}

		c = pdf_peek_byte(doc->file);
		if ( c == 't' ){
			unsigned char buff[1024];
			int n, i;
			n = pdf_read(doc->file, buff, sizeof(buff));
			for (i=0; i<n; i++){
				if ( (memcmp(buff + i, "Size", 4) == 0) ){
					i += 4;
					while (i < n && (buff[i] <= 32 || buff[i] >= 127)){
						i++;
					}
					size=0;
					while (i < n && buff[i] >= '0' && buff[i] <= '9'){
						size=size * 10 + (buff[i++] - '0');
					}
					break;
				}
			}
		}else{
			printf("Trailer not found in document\n");
			exit(0);
		}


		//Restoring the offset recorded
		pdf_seek(doc->file, offsets, PDFSEEK_SET);
		return size;
	}


	C_MODE_END
