/**
*	@file			PDFStream.c
*	@brief			Functions Relating to Streams
**/

	#include <PDFTextExtraction.h>

	C_MODE_START

	/***************************** Starting Prototypes ********************/
	void pdf_clear_stack(pdf_contents *contents);
	void pdf_create_stack(pdf_contents *contents);
	void pdf_set_line_font(pdf_contents *contents);
	void pdf_create_newLine(pdf_contents *contents);
	void pdf_beign_text(pdf_contents *contents, pdf_lexbuf *buf);
	int pdf_run_keyword(pdf_contents *contents PDFUnused, char *buf);
	pdf_content_line *pdf_get_last_content_line(pdf_contents *contents);
	void pdf_process_stream(pdf_document *doc, pdf_obj *resources, const char *name, int number);
	/***************************** Ending Prototypes **********************/


	/***************************** Global Variables ********************/
	#define 	A(a) 		(a)
	#define 	B(a,b) 		(a | b << 8)
	#define 	C(a,b,c) 	(a | b << 8 | c << 16)
	#define 	nelem(x) 	(int)(sizeof(x)/sizeof((x)[0]))
	/***************************** Global Variables ********************/

	pdf_content_line *pdf_get_last_content_line(pdf_contents *contents){
		pdf_content_line *ret=NULL;

		pdf_content_line	*current, *next;
		for(current=contents->details; current; ){
			next=current->next;
			ret=current;
			current=next;
		}

		return ret;
	}

	void pdf_set_line_font(pdf_contents *contents){
		if ( contents->allfonts ){
			pdf_content_fonts	*current, *next;

			for(current=contents->allfonts; current; ){
				next=current->next;
				if ( strcmp(contents->stack->name, current->ref_name) == 0 ){
					pdf_content_line	*last;
					last=pdf_get_last_content_line(contents);
					if ( last->len ){
						printf("Allocation New PartNumber of Line\n");
						exit(0);
					}
					if ( last->font ){
						PDFFree(last->font);
					}
					last->font=(pdf_content_line_font*)PDFMalloc(sizeof(pdf_content_line_font));
					memset(last->font, 0, sizeof(pdf_content_line_font));

					pdf_strlcpy(last->font->fontName, current->name, sizeof(last->font->fontName));
					pdf_strlcpy(last->font->type, current->type, sizeof(last->font->type));
					last->font->is_italic=current->is_italic;
					last->font->is_bold=current->is_bold;
					break;
				}
				current=next;
			}
		}
	}

	void pdf_create_newLine(pdf_contents *contents){
		pdf_content_line	*line;

		line=(pdf_content_line*)PDFMalloc(sizeof(pdf_content_line));
		memset(line, 0, sizeof(pdf_content_line));

		line->LineNumber=contents->TotalLines++;

		if ( contents->details ){
			if ( contents->details->next ){
				line->prev=contents->details->prev;
				contents->details->prev->next=line;
				contents->details->prev=line;
			}else{
				line->prev=contents->details;
				contents->details->prev=line;
				contents->details->next=line;
			}
		}else{
			contents->details=line;
		}
	}

	int pdf_run_keyword(pdf_contents *contents PDFUnused, char *buf){
		int key;

		key = buf[0];
		if (buf[1])	{
			key |= buf[1] << 8;
			if (buf[2]){
				key |= buf[2] << 16;
				if (buf[3]){
					key = 0;
				}
			}
		}

		switch (key){
			case B('T', 'f'):
				pdf_set_line_font(contents);
				break;
			default:
				printf("Current matching with=%s\n", buf);
				exit(0);
		}
		return 0;
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
				default:
					if ( tok == PDF_TOK_KEYWORD){
						printf("PDF_TOK_KEYWORD default %s\n", buf->scratch);
					}else{
						printf("Token=%d\ndefault %s\n", tok, buf->scratch);
					}
					exit(0);
			}
		}while (tok != PDF_TOK_EOF);
	}

	void pdf_create_stack(pdf_contents *contents){
		pdf_contents_stack *stack;

		stack=(pdf_contents_stack*)PDFMalloc(sizeof(pdf_contents_stack));
		memset(stack, 0, sizeof(pdf_contents_stack));

		if ( contents->stack ){
			PDFFree(contents->stack);
			contents->stack=stack;
		}else{
			contents->stack=stack;
		}
	}

	void pdf_clear_stack(pdf_contents *contents){
		int i;

		//Drop the contents object
		pdf_drop_obj(contents->stack->obj);
		contents->stack->obj = NULL;
		contents->stack->name[0] = 0;
		contents->stack->string_len = 0;
		for (i = 0; i < contents->stack->top; i++){
			contents->stack->stack[i] = 0;
		}
		contents->stack->top = 0;
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
