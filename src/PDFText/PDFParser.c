/**
*	@file		:	PDFParser.c
*	@brief		:	Parser for PDF File
**/

	#include <PDFTextExtraction.h>

	C_MODE_START


	/***************************** Starting Prototypes ********************/
	fz_rect *pdf_to_rect(pdf_obj *array, fz_rect *r);
	pdf_obj *pdf_parse_array(pdf_document *doc, pdf_stream *file, pdf_lexbuf *buf);
	pdf_trailer *pdf_parse_dict(pdf_document *doc, pdf_stream *file, pdf_lexbuf *buf);
	pdf_obj *pdf_parse_ind_obj(pdf_document *doc, pdf_stream *file, pdf_lexbuf *buf, int *onum, int *ogen, int *ostmofs);
	/***************************** Ending Prototypes **********************/

	/***************************** Global Variables ********************/
	/***************************** Global Variables ********************/

	fz_rect *pdf_to_rect(pdf_obj *array, fz_rect *r){
		float a = pdf_to_real(pdf_array_get(array, 0));
		float b = pdf_to_real(pdf_array_get(array, 1));
		float c = pdf_to_real(pdf_array_get(array, 2));
		float d = pdf_to_real(pdf_array_get(array, 3));
		r->x0 = fz_min(a, c);
		r->y0 = fz_min(b, d);
		r->x1 = fz_max(a, c);
		r->y1 = fz_max(b, d);
		return r;
	}


	pdf_obj *pdf_parse_ind_obj(pdf_document *doc PDFUnused, pdf_stream *file, pdf_lexbuf *buf, int *onum PDFUnused, int *ogen PDFUnused, int *ostmofs PDFUnused){
		pdf_obj *obj = NULL;
		pdf_token tok;
		int num = 0, gen = 0, stm_ofs = 0, a, b;

		tok = pdf_lex(file, buf);
		if (tok != PDF_TOK_INT){
			printf("abc expected object number\n");
			exit(0);
		}
		num = buf->i;

		tok = pdf_lex(file, buf);
		if (tok != PDF_TOK_INT){
			printf("def expected generation number (%d ? obj)\n", num);
			exit(0);
		}
		gen = buf->i;

		tok = pdf_lex(file, buf);
		if (tok != PDF_TOK_OBJ){
			printf("expected 'obj' keyword (%d %d ?)\n", num, gen);
			exit(0);
		}
		tok = pdf_lex(file, buf);
		switch (tok){
			case PDF_TOK_OPEN_ARRAY:
				printf("PDF_TOK_OPEN_ARRAY\n");
				exit(0);
			case PDF_TOK_OPEN_DICT:
				obj = pdf_parse_dict(doc, file, buf);
				break;
			case PDF_TOK_NAME:
				printf("PDF_TOK_NAME\n");
				exit(0);
			case PDF_TOK_REAL:
				printf("abc PDF_TOK_REAL\n");
				exit(0);
			case PDF_TOK_STRING:
				printf("PDF_TOK_STRING\n");
				exit(0);
			case PDF_TOK_TRUE:
				printf("PDF_TOK_TRUE\n");
				exit(0);
			case PDF_TOK_FALSE:
				printf("PDF_TOK_FALSE\n");
				exit(0);
			case PDF_TOK_NULL:
				printf("PDF_TOK_NULL\n");
				exit(0);
			case PDF_TOK_INT:
				a = buf->i;
				tok = pdf_lex(file, buf);
				if (tok == PDF_TOK_STREAM || tok == PDF_TOK_ENDOBJ){
					obj = pdf_new_int(a);
					goto skip;
				}
				if (tok == PDF_TOK_INT){
					b = buf->i;
					tok = pdf_lex(file, buf);
					if (tok == PDF_TOK_R){
						obj = pdf_new_indirect(a, b);
						break;
					}
				}
				printf("expected 'R' keyword (%d %d R)", num, gen);
				exit(0);
			case PDF_TOK_ENDOBJ:
				printf("PDF_TOK_ENDOBJ\n");
				exit(0);
			default:
				printf("default\n");
				exit(0);
		}

		tok = pdf_lex(file, buf);
	skip:
		if (tok == PDF_TOK_STREAM){
			int c = pdf_read_byte(file);

			while (c == ' '){
				c = pdf_read_byte(file);
			}
			if (c == '\r'){
				c = pdf_peek_byte(file);
				if (c != '\n'){
					printf("line feed missing after stream begin marker (%d %d R)", num, gen);
					exit(0);
				}else{
					pdf_read_byte(file);
				}
			}
			stm_ofs = pdf_tell(file);
		}else if (tok == PDF_TOK_ENDOBJ){
			stm_ofs = 0;
		}else{
			printf("expected 'endobj' or 'stream' keyword (%d %d R)", num, gen);
			exit(0);
			stm_ofs = 0;
		}

		if (onum) *onum = num;
		if (ogen) *ogen = gen;
		if (ostmofs) *ostmofs = stm_ofs;

		return obj;
	}


	pdf_obj *pdf_parse_array(pdf_document *doc, pdf_stream *file, pdf_lexbuf *buf){
		pdf_obj *op = NULL;
		PDFUnused pdf_obj *ary = NULL, *obj = NULL;
		pdf_token tok;
		PDFUnused int a = 0, b = 0, n = 0;

		ary = pdf_new_array(doc, 4);

		while (1){
			tok = pdf_lex(file, buf);

			if (tok != PDF_TOK_INT && tok != PDF_TOK_R){
				if (n > 0){
					obj = pdf_new_int(a);
					pdf_array_push(doc, ary, obj);
					pdf_drop_obj(obj);
					obj = NULL;
				}
				if (n > 1){
					obj = pdf_new_int(b);
					pdf_array_push(doc, ary, obj);
					pdf_drop_obj(obj);
					obj = NULL;
				}
				n=0;
			}
			if (tok == PDF_TOK_INT && n == 2){
				obj = pdf_new_int(a);
				pdf_array_push(doc, ary, obj);
				pdf_drop_obj(obj);
				obj = NULL;
				a = b;
				n --;
			}
			switch (tok){
				case PDF_TOK_CLOSE_ARRAY:
					op = ary;
					goto end;
				case PDF_TOK_INT:
					if (n == 0)
						a = buf->i;
					if (n == 1)
						b = buf->i;
					n ++;
					break;
				case PDF_TOK_R:
					if (n != 2){
						printf("cannot parse indirect reference in array\n");
						exit(0);
					}
					obj = pdf_new_indirect(a, b);
					pdf_array_push(doc, ary, obj);
					pdf_drop_obj(obj);
					obj = NULL;
					n = 0;
					break;
				case PDF_TOK_OPEN_ARRAY:
					printf("PDF_TOK_OPEN_ARRAY\n");
					exit(0);
				case PDF_TOK_OPEN_DICT:
					printf("PDF_TOK_OPEN_DICT\n");
					exit(0);
				case PDF_TOK_NAME:
					obj = pdf_new_name(buf->scratch);
					pdf_array_push(doc, ary, obj);
					pdf_drop_obj(obj);
					obj = NULL;
					break;
				case PDF_TOK_REAL:
					obj = pdf_new_real(buf->f);
					pdf_array_push(doc, ary, obj);
					pdf_drop_obj(obj);
					obj = NULL;
					break;
				case PDF_TOK_STRING:
					obj = pdf_new_string(buf->scratch, buf->len);
					pdf_array_push(doc, ary, obj);
					pdf_drop_obj(obj);
					obj = NULL;
					break;
				case PDF_TOK_TRUE:
					printf("PDF_TOK_TRUE\n");
					exit(0);
				case PDF_TOK_FALSE:
					printf("PDF_TOK_FALSE\n");
					exit(0);
				case PDF_TOK_NULL:
					printf("PDF_TOK_NULL\n");
					exit(0);
				default:
					printf("cannot parse token in array\n");
					exit(0);
					break;
			}
		}
end:
		return op;
	}

	pdf_trailer *pdf_parse_dict(pdf_document *doc, pdf_stream *file, pdf_lexbuf *buf){
		pdf_trailer *dict=NULL;
		PDFUnused pdf_obj *key=NULL;
		PDFUnused pdf_obj *val=NULL;
		pdf_token tok;
		PDFUnused int a, b;

		//Creating New Objects Dictionary
		dict = pdf_new_dict();

		//starting parsing dictionary
		while (1){
			tok = pdf_lex(file, buf);

		skip:
			//if token is closing dictionary parsing is compeleted
			if (tok == PDF_TOK_CLOSE_DICT){
				break;
			}

			/* for BI .. ID .. EI in content streams */
			if (tok == PDF_TOK_KEYWORD && !strcmp(buf->scratch, "ID")){
				break;
			}

			if (tok != PDF_TOK_NAME){
				printf("invalid key in dict\n");
				exit(0);
			}

			//Parsing key of dictionary
			key=pdf_new_name(buf->scratch);

			tok = pdf_lex(file, buf);

			switch (tok){
				case PDF_TOK_OPEN_ARRAY:
					val = pdf_parse_array(doc, file, buf);
					break;
				case PDF_TOK_OPEN_DICT:
					val = pdf_parse_dict(doc, file, buf);
					break;
				case PDF_TOK_NAME:
					val = pdf_new_name(buf->scratch);
					break;
				case PDF_TOK_REAL:
					val = pdf_new_real(buf->f);
					break;
				case PDF_TOK_STRING:
					val = pdf_new_string(buf->scratch, buf->len);
					break;
				case PDF_TOK_TRUE:
					val = pdf_new_bool(1);
					break;
				case PDF_TOK_FALSE:
					printf("PDF_TOK_FALSE\n");
					exit(0);
					break;
				case PDF_TOK_NULL:
					printf("PDF_TOK_NULL\n");
					exit(0);
					break;
				case PDF_TOK_INT:
					a = buf->i;
					tok = pdf_lex(file, buf);
					if (
						tok == PDF_TOK_CLOSE_DICT ||
						tok == PDF_TOK_NAME ||
						(tok == PDF_TOK_KEYWORD &&
						!strcmp(buf->scratch, "ID"))
					){
						val = pdf_new_int(a);
						pdf_dict_put(doc, dict, key, val);
						pdf_drop_obj(val);
						val=NULL;
						pdf_drop_obj(key);
						key=NULL;
						goto skip;
					}
					if (tok == PDF_TOK_INT){
						b = buf->i;
						tok = pdf_lex(file, buf);
						if (tok == PDF_TOK_R){
							val = pdf_new_indirect(a, b);
							break;
						}
					}
					printf("invalid indirect reference in dict\n");
					exit(0);
					break;
				default:
					printf("unknown token type\n");
					exit(0);
					break;
			}
			pdf_dict_put(doc, dict, key, val);
			pdf_drop_obj(val);
			val = NULL;
			pdf_drop_obj(key);
			key = NULL;
		}

		return dict;
	}

	C_MODE_END
