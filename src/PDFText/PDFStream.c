/**
*	@file			PDFStream.c
*	@brief			Functions Relating to Streams
**/

	#include <PDFTextExtraction.h>

	C_MODE_START

	/***************************** Starting Prototypes ********************/
	void pdf_process_stream(pdf_document *doc, pdf_obj *resources, const char *name, int number);
	/***************************** Ending Prototypes **********************/


	/***************************** Global Variables ********************/
	/***************************** Global Variables ********************/

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
