/**
*	@file			PDFStream.c
*	@brief			Functions Relating to Streams
**/

	#include <PDFTextExtraction.h>

	C_MODE_START

	/***************************** Starting Prototypes ********************/
	void pdf_beign_text(pdf_contents *contents, pdf_lexbuf *buf);
	int pdf_check_character(pdf_contents *contents, int character);
	void pdf_string_push(pdf_contents *contents, char *buf, int len);
	void pdf_process_stream(pdf_document *doc, pdf_obj *resources, const char *name, int number);
	/***************************** Ending Prototypes **********************/


	/***************************** Global Variables ********************/
	/***************************** Global Variables ********************/

	int pdf_check_character(pdf_contents *contents PDFUnused, int character){
		//Get the last line resources before creating new block
		pdf_content_line	*last;
		last=pdf_get_last_content_line(contents);

		pdf_content_fonts *current, *next;

		for(current=contents->allfonts; current; ){
			next=current->next;
			if ( strcmp(current->name, last->font->fontName) == 0 ){
				printf("%s\n", last->font->fontName);
				if ( current->unicode ){
					printf("Do things with Unicode\n");
				}else{
					printf("Do things with Encoding\n");
					exit(0);
				}
				break;
			}
			current=next;
		}
		return character;
	}

	void pdf_string_push(pdf_contents *contents, char *buf, int len){
		int i;

		//Get the last line resources before creating new block
		pdf_content_line	*last;
		last=pdf_get_last_content_line(contents);

		for(i=0; i<len; i++){
			if ( last->len < 512 ){
				last->text[last->len++]=(char) pdf_check_character(contents, buf[i]);
			}else{
				printf("No Space Available\n");
				exit(0);
			}
		}
		printf("LineNumber=%03d; PartNumber=%03d; TextLength=%03d;\n \
				====================================\n%s\n====================================\n",
				last->LineNumber, last->PartNumber, last->len, last->text);
	}

	void pdf_beign_text(pdf_contents *contents, pdf_lexbuf *buf){
		pdf_token tok = PDF_TOK_ERROR;

		//Creating A newLine with Resources
		pdf_create_newLine(contents);

		pdf_read_fonts(contents);

		//Creating stack for contents
		pdf_create_stack(contents);

		do{
			tok = pdf_lex(contents->file, buf);
			switch(tok){
				case PDF_TOK_ENDSTREAM:
				case PDF_TOK_EOF:
					tok = PDF_TOK_EOF;
					break;
				case PDF_TOK_REAL:
					if (contents->stack->top < nelem(contents->stack->stack)) {
						contents->stack->stack[contents->stack->top] = buf->f;
						contents->stack->top ++;
					}else{
						printf("stack overflow\n");
						exit(0);
					}
					break;
				case PDF_TOK_INT:
					if (contents->stack->top < nelem(contents->stack->stack)) {
						contents->stack->stack[contents->stack->top] = (float)buf->i;
						contents->stack->top ++;
					}else{
						printf("stack overflow\n");
						exit(0);
					}
					break;
				case PDF_TOK_NAME:
					if (contents->stack->name[0]){
						printf("contents->name[0] having already object");
						exit(0);
					}else{
						pdf_strlcpy(contents->stack->name, buf->scratch, sizeof(contents->stack->name));
					}
					break;
				case PDF_TOK_KEYWORD:
					if (pdf_run_keyword(contents, buf->scratch)){
						tok = PDF_TOK_EOF;
					}
					pdf_clear_stack(contents);
					break;
				case PDF_TOK_OPEN_ARRAY:
					printf("PDF_TOK_OPEN_ARRAY\n");
					exit(0);
				case PDF_TOK_OPEN_DICT:
					printf("PDF_TOK_OPEN_DICT\n");
					exit(0);
				case PDF_TOK_STRING:
					pdf_string_push(contents, buf->scratch, buf->len);
					break;
				default:
					printf("Token=%d\ndefault %s\n", tok, buf->scratch);
					exit(0);
			}
		}while (tok != PDF_TOK_EOF);
	}

	void pdf_process_stream(pdf_document *doc, pdf_obj *resources, const char *name, int number){
		pdf_contents *contents;

		contents=(pdf_contents*)PDFMalloc(sizeof(*contents));
		memset(contents, 0, sizeof(*contents));

		contents->file=pdf_keep_stream(pdf_open_file(name));
		contents->doc=doc;

		if ( resources->kind == PDF_INDIRECT ){
			contents->resources=pdf_resolve_indirect(doc, resources);
		}else{
			contents->resources=resources;
		}
		contents->PageNo=number+1;

		//As we are hear to work only for text extraction
		//thus our point of work would start when BT i.e.
		//Begin of text Block found and do working until ET is
		//found i.e. End of Text Block Block
		pdf_lexbuf *buf=&contents->doc->lexbuf.base;
		pdf_token tok;
		do{
			tok = pdf_lex(contents->file, buf);
			if ( tok == PDF_TOK_KEYWORD ){
				if ( strcmp(buf->scratch, "BT") == 0 ){
					printf("Found Block of Text at Page No.%d\n", contents->PageNo);
					pdf_beign_text(contents, buf);
				}
			}
		}while (tok != PDF_TOK_EOF);

		pdf_free_stream(contents->file);
		if ( doc->contents ){
			if ( doc->contents->next ){
				contents->prev=doc->contents->prev;
				doc->contents->prev->next=contents;
				doc->contents->prev=contents;
			}else{
				contents->prev=doc->contents;
				doc->contents->prev=contents;
				doc->contents->next=contents;
			}
		}else{
			doc->contents=contents;
		}
	}


	C_MODE_END
