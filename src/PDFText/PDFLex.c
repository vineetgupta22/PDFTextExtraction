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
*	@file		:	PDFLex.c
*	@brief		:	Functions PDF Lex Elements processing
**/

	#include <PDFTextExtraction.h>

	C_MODE_START


	/***************************** Starting Prototypes ********************/
	void lex_white(pdf_stream *f);
	pdf_token pdf_token_from_keyword(char *key);
	void lex_name(pdf_stream *f, pdf_lexbuf *buf);
	pdf_token pdf_lex(pdf_stream *f, pdf_lexbuf *buf);
	void pdf_lexbuf_init(pdf_lexbuf *lex_buffer, int size);
	/***************************** Ending Prototypes **********************/

	/***************************** Global Variables ********************/
	/***************************** Global Variables ********************/

	/**
	*	@fn			lex_white(pdf_stream *f)
	*	@param[in]	f		PDF Stream
	*	@brief		During token generation or parsing the white are
	*	ignoring done by this function
	**/
	void lex_white(pdf_stream *f){
		int c;
		do {
			c = pdf_read_byte(f);
		} while ((c <= 32) && (pdf_iswhite(c)));
		if (c != EOF){
			pdf_unread_byte(f);
		}
	}

	/**
	*	@fn			lex_name(pdf_stream *f, pdf_lexbuf *buf)
	*	@param[in]	f		PDF Stream
	*	@param[in]	buf		Lex Buffer for doing Lex Parser
	*	@brief		Parsing the PDF Token Name
	**/
	void lex_name(pdf_stream *f, pdf_lexbuf *buf){
		char *s = buf->scratch;
		int n = buf->size;

		while (n > 1){
			int c = pdf_read_byte(f);
			switch (c){
				case IS_WHITE:
				case IS_DELIM:
					pdf_unread_byte(f);
					goto end;
				case EOF:
					goto end;
				case '#':{
					printf("recieved #");
					exit(0);
				}
				default:
					*s++=(char)c;
					n--;
					break;
			}
		}
end:
		*s = '\0';
		buf->len = s - buf->scratch;
	}

	/**
	*	@fn			pdf_token_from_keyword(char *key)
	*	@param[in]	key		Token Key Name
	*	@brief		Returning the token name of provided key
	**/
	pdf_token pdf_token_from_keyword(char *key){
		switch (*key){
			case 'R':
				if (!strcmp(key, "R")){
					return PDF_TOK_R;
				}
				break;
			case 't':
				if (!strcmp(key, "true")){
					return PDF_TOK_TRUE;
				}
				if (!strcmp(key, "trailer")){
					return PDF_TOK_TRAILER;
				}
				break;
			case 'f':
				if (!strcmp(key, "false")){
					return PDF_TOK_FALSE;
				}
				break;
			case 'n':
				if (!strcmp(key, "null")){
					return PDF_TOK_NULL;
				}
				break;
			case 'o':
				if (!strcmp(key, "obj")){
					return PDF_TOK_OBJ;
				}
				break;
			case 'e':
				if (!strcmp(key, "endobj")){
					return PDF_TOK_ENDOBJ;
				}
				if (!strcmp(key, "endstream")){
					return PDF_TOK_ENDSTREAM;
				}
				break;
			case 's':
				if (!strcmp(key, "stream")){
					return PDF_TOK_STREAM;
				}
				if (!strcmp(key, "startxref")){
					return PDF_TOK_STARTXREF;
				}
				break;
			case 'x':
				if (!strcmp(key, "xref")){
					return PDF_TOK_XREF;
				}
				break;
			default:
				break;
		}
		return PDF_TOK_KEYWORD;
	}

	/**
	*	@fn						pdf_lexbuf_init(pdf_lexbuf *lex_buffer, int size)
	*	@param	[in]	lex_buffer		PDF Lex Buffer Structure
	*	@param	[in]	size			Maximum size of buffer to hold
	*	@brief					Initialization of lex buffer structure for reading
	*	and parse the elements of PDF.
	**/
	void pdf_lexbuf_init(pdf_lexbuf *lex_buffer, int size){
		lex_buffer->size=size;
		lex_buffer->base_size=size;
		lex_buffer->len=0;
		lex_buffer->scratch = &lex_buffer->buffer[0];
	}

	/**
	*	@fn			pdf_token pdf_lex(pdf_stream *f, pdf_lexbuf *buf)
	*	@param[in]	f		PDF Stream
	*	@param[in]	buf		Lex Buffer for doing Lex Parser
	*	@brief		The offset in the file is already set we have to just
	*	parse the xref section and return
	**/
	pdf_token pdf_lex(pdf_stream *f, pdf_lexbuf *buf){
		while (1){
			int c = pdf_read_byte(f);
			switch (c){
				case EOF:
					return PDF_TOK_EOF;
				case IS_WHITE:
					lex_white(f);
					break;
				case '%':
					printf("lex_comment(f);");
					exit(0);
					break;
				case '/':
					printf("lex_name(f, buf);");
					printf(" return PDF_TOK_NAME; ");
					break;
				case '(':
					printf("return lex_string(f, buf);");
					exit(0);
					break;
				case ')':
					printf("lexical error (unexpected ')')");
					continue;
				case '<':
					c = pdf_read_byte(f);
					if (c == '<'){
						return PDF_TOK_OPEN_DICT;
					}else{
						printf("pdf_unread_byte(f);");
						printf("return lex_hex_string(f, buf);");
						exit(0);
					}
				case '>':
					c = pdf_read_byte(f);
					if (c == '>'){
						return PDF_TOK_CLOSE_DICT;
					}
					printf("lexical error (unexpected '>')\n if (c == EOF) return PDF_TOK_EOF; fz_unread_byte(f); continue;");
					exit(0);
				case '[':
					return PDF_TOK_OPEN_ARRAY;
				case ']':
					return PDF_TOK_CLOSE_ARRAY;
				case '{':
					return PDF_TOK_OPEN_BRACE;
				case '}':
					return PDF_TOK_CLOSE_BRACE;
				case IS_NUMBER:
					printf("return lex_number(f, buf, c);");
					exit(0);
					break;
				default:
					pdf_unread_byte(f);
					lex_name(f, buf);
					return pdf_token_from_keyword(buf->scratch);
			}
		}
	}

	C_MODE_END
