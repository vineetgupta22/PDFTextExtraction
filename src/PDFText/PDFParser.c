/**
*	@file		:	PDFParser.c
*	@brief		:	Parser for PDF File
**/

	#include <PDFTextExtraction.h>

	C_MODE_START


	/***************************** Starting Prototypes ********************/
	pdf_obj *pdf_parse_array(pdf_document *doc, pdf_stream *file, pdf_lexbuf *buf);
	pdf_trailer *pdf_parse_dict(pdf_document *doc, pdf_stream *file, pdf_lexbuf *buf);
	/***************************** Ending Prototypes **********************/

	/***************************** Global Variables ********************/
	/***************************** Global Variables ********************/

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
					printf("n>0\n");
					exit(0);
				}
				if (n > 1){
					printf("n>1\n");
					exit(0);
				}
				n=0;
			}
			if (tok == PDF_TOK_INT && n == 2){
				printf("n==2\n");
				exit(0);
			}
			switch (tok){
				case PDF_TOK_CLOSE_ARRAY:
					op = ary;
					goto end;
				case PDF_TOK_INT:
					printf("PDF_TOK_INT\n");
					exit(0);
				case PDF_TOK_R:
					printf("PDF_TOK_R\n");
					exit(0);
				case PDF_TOK_OPEN_ARRAY:
					printf("PDF_TOK_OPEN_ARRAY\n");
					exit(0);
				case PDF_TOK_OPEN_DICT:
					printf("PDF_TOK_OPEN_DICT\n");
					exit(0);
				case PDF_TOK_NAME:
					printf("PDF_TOK_NAME\n");
					exit(0);
				case PDF_TOK_REAL:
					printf("PDF_TOK_REAL\n");
					exit(0);
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
					printf("PDF_TOK_OPEN_DICT\n");
					exit(0);
					break;
				case PDF_TOK_NAME:
					printf("PDF_TOK_NAME\n");
					exit(0);
					break;
				case PDF_TOK_REAL:
					printf("PDF_TOK_REAL\n");
					exit(0);
					break;
				case PDF_TOK_STRING:
					printf("PDF_TOK_STRING\n");
					exit(0);
					break;
				case PDF_TOK_TRUE:
					printf("PDF_TOK_TRUE\n");
					exit(0);
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
