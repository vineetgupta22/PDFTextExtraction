/**
*	@file			PDFStream.c
*	@brief			Functions Relating to Streams
**/

	#include <PDFTextExtraction.h>

	C_MODE_START

	/***************************** Starting Prototypes ********************/
	void pdf_create_stack(pdf_contents *contents);
	void pdf_beign_text(pdf_contents *contents, pdf_lexbuf *buf);
	void pdf_process_stream(pdf_document *doc, pdf_obj *resources, const char *name, int number);
	/***************************** Ending Prototypes **********************/


	/***************************** Global Variables ********************/
	#define 	nelem(x) 	(int)(sizeof(x)/sizeof((x)[0]))
	/***************************** Global Variables ********************/

	void pdf_beign_text(pdf_contents *contents, pdf_lexbuf *buf){
		pdf_token tok = PDF_TOK_ERROR;

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
