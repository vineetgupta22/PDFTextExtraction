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
	int unhex(int ch);
	void lex_white(pdf_stream *f);
	pdf_token pdf_token_from_keyword(char *key);
	int lex_string(pdf_stream *f, pdf_lexbuf *lb);
	void lex_name(pdf_stream *f, pdf_lexbuf *buf);
	int lex_hex_string(pdf_stream *f, pdf_lexbuf *lb);
	pdf_token pdf_lex(pdf_stream *f, pdf_lexbuf *buf);
	int lex_number(pdf_stream *f, pdf_lexbuf *buf, int c);
	void pdf_lexbuf_init(pdf_lexbuf *lex_buffer, int size);
	/***************************** Ending Prototypes **********************/

	/***************************** Global Variables ********************/
	/***************************** Global Variables ********************/

	int lex_string(pdf_stream *f, pdf_lexbuf *lb){
		char *s = lb->scratch;
		char *e = s + lb->size;
		int c ;
		int bal = 1;

		while (1){
			if (s == e){
				printf("s==e\n");
				exit(0);
			}
			c = pdf_read_byte(f);
			switch (c){
				case EOF:
					goto end;
				case '(':
					bal++;
					*s++ = (char)c;
					break;
				case ')':
					bal --;
					if (bal == 0)
						goto end;
					*s++ = (char)c;
					break;
				case '\\':
					printf("\\\n");
					exit(0);
				default:
					*s++ = (char)c;
					break;
			}
		}
end:
		lb->len = s - lb->scratch;
		return PDF_TOK_STRING;
	}

	int unhex(int ch){
		if (ch >= '0' && ch <= '9') return ch - '0';
		if (ch >= 'A' && ch <= 'F') return ch - 'A' + 0xA;
		if (ch >= 'a' && ch <= 'f') return ch - 'a' + 0xA;
		return 0;
	}

	int lex_hex_string(pdf_stream *f, pdf_lexbuf *lb){
		char *s = lb->scratch;
		char *e = s + lb->size;
		int a = 0, x = 0;
		int c;

		while (1){
			if (s == e){
				printf("s==e\n");
				exit(0);
			}
			c = pdf_read_byte(f);
			switch(c){
				case IS_WHITE:
					break;
				case IS_HEX:
					if (x){
						*s++ = (char)(a * 16 + unhex(c));
						x = ~x;
					}else{
						a = unhex(c);
						x = ~x;
					}
					break;
				case '>':
				case EOF:
					goto end;
				default:
					printf("ignoring invalid character in hex string");
					exit(0);
			}
		}
	end:
		lb->len = s - lb->scratch;
		return PDF_TOK_STRING;
	}


	/**
	*	@fn			lex_number(pdf_stream *f, pdf_lexbuf *buf, int c)
	*	@param[in]	f		PDF Stream
	*	@param[in]	buf		Lex Buffer for doing Lex Parser
	*	@param[in]	c		Already Read byte
	*	@brief		During token generation or parsing the white are
	*	ignoring done by this function
	**/
	int lex_number(pdf_stream *f, pdf_lexbuf *buf, int c){
		int neg = 0;
		int i = 0;
		int n;
		int d;
		float v;

		/* Initially we might have +, -, . or a digit */
		switch (c){
			case '.':
				goto loop_after_dot;
			case '-':
				neg = 1;
				break;
			case '+':
				break;
			default:
				//Assumption that it is digit
				i = c - '0';
				break;
		}

		while(1){
			c = pdf_read_byte(f);
			switch (c){
				case '.':
					goto loop_after_dot;
				case RANGE_0_9:
					i = 10*i + c - '0';
					break;
				default:
					pdf_unread_byte(f);
				case EOF:
					if (neg)
						i = -i;
				buf->i = i;
				return PDF_TOK_INT;
			}
		}

	loop_after_dot:
		/* In here, we've seen a dot, so can accept just digits */

		n = 0;
		d = 1;
		while (1){
			c = pdf_read_byte(f);
			switch (c){
				case RANGE_0_9:
					if (d >= INT_MAX/10){
						printf("goto underflow;");
						exit(0);
					}
					n = n*10 + (c - '0');
					d *= 10;
					break;
				default:
					pdf_unread_byte(f);
				case EOF:
					v = (float)i + ((float)n / (float)d);
					if (neg)
						v = -v;
					buf->f = v;
					return PDF_TOK_REAL;
			}
		}
	}

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
					lex_name(f, buf);
					return PDF_TOK_NAME;
					break;
				case '(':
					return lex_string(f, buf);
				case ')':
					printf("lexical error (unexpected ')')");
					continue;
				case '<':
					c = pdf_read_byte(f);
					if (c == '<'){
						return PDF_TOK_OPEN_DICT;
					}else{
						pdf_unread_byte(f);
						return lex_hex_string(f, buf);
					}
					printf("return lex_hex_string(f, buf);");
					exit(0);
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
					return lex_number(f, buf, c);
				default:
					pdf_unread_byte(f);
					lex_name(f, buf);
					return pdf_token_from_keyword(buf->scratch);
			}
		}
	}

	C_MODE_END
