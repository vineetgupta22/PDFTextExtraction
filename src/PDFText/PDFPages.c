/**
*	@file		:	PDFPages.c
*	@brief		:	For Loading PDF Pages
**/

	#include <PDFTextExtraction.h>

	C_MODE_START


	/***************************** Starting Prototypes ********************/
	void pdf_load_page(pdf_document *doc, int number);
	extern pdf_stream *pdf_open_filter(pdf_stream *chain, pdf_document *doc, pdf_obj *stmobj, int num, int gen, int offset);
	/***************************** Ending Prototypes **********************/

	/***************************** Global Variables ********************/
	/***************************** Global Variables ********************/

	void pdf_load_page(pdf_document *doc, int number){
		printf("Reached to load page number = %03d\n", number+1);
		pdf_obj *hit=NULL;
		pdf_obj *kids = NULL;
		int len, i;
		int stack_max = 16;
		int stack_len = 0;
		pdf_obj *local_stack[16];
		pdf_obj **stack = &local_stack[0];
		int skip=number;
		pdf_xref *xref = &doc->xref_sections[0];

		//Getting the root object which we already done.
		pdf_obj *root = pdf_dict_gets(doc, xref->trailer, "Root");

		//From root to pages already resolved indirect reference
		pdf_obj *node = pdf_dict_gets(doc, root, "Pages");

		if (!node){
			printf("cannot find page tree\n");
			exit(0);
		}

		do{
			kids = pdf_dict_gets(doc, node, "Kids");
			len = pdf_array_len(kids);

			if (len == 0){
				printf("Malformed pages tree\n");
				exit(0);
			}

			/* Every node we need to unmark goes into the stack */
			if (stack_len == stack_max){
				printf("stack_len==stack_max\n");
				exit(0);
			}

			stack[stack_len++] = node;

			if (pdf_mark_obj(doc, node)){
				printf("cycle in page tree\n");
				exit(0);
			}

			for (i = 0; i < len; i++){
				//Get the kid page
				pdf_obj *kid = pdf_array_get(kids, i);

				char *type = pdf_to_name(pdf_dict_gets(doc, kid, "Type"));

				if ( type ){
					if ( !strcmp(type, "Pages") ){
						if ( pdf_dict_gets(doc, kid, "Kids") && !pdf_dict_gets(doc, kid, "MediaBox") ){
							int count = pdf_to_int(doc, pdf_dict_gets(doc, kid, "Count"));
							if (skip < count){
								node = kid;
								break;
							}else{
								skip -= count;
							}
						}else{
							printf("non-page object in page tree (%s)\n", type);
							exit(0);
						}
					}else{
						if ( strcmp(type, "Page") != 0 ){
							printf("non-page object in page tree (%s)\n", type);
							exit(0);
						}
						if (skip == 0){
							hit = kid;
							break;
						}else{
							skip--;
						}
					}
				}else{
					printf("No page type found\n");
					exit(0);
				}
			}
		}while (hit == NULL);

		for (i = stack_len; i > 0; i--){
			pdf_unmark_obj(doc, stack[i-1]);
		}
		if (stack != &local_stack[0]){
			printf("fz_free(ctx, stack)\n");
			exit(0);
		}

		//hit is having the page object
		pdf_obj *pageobj, *obj;

		//Resolving pageobj
		pageobj = pdf_resolve_indirect(doc, hit);

		//Any measurement unit defined in document
		obj = pdf_dict_gets(doc, pageobj, "UserUnit");
		if ( obj ){
			printf("\n\nobj->kind=%c\n\n", obj->kind);
			exit(0);
		}
		
		//Getting the resources from current object
		obj = pdf_dict_gets(doc, pageobj, "Contents");

		printf("obj->kind=%c", obj->kind);

		pdf_xref_entry *entry;
		entry = pdf_cache_object(doc, obj->u.r.num, obj->u.r.gen);

		if ( entry->offsets ){
			char *stream;

			int lenp = pdf_to_int(doc, pdf_dict_gets(doc, obj, "Length"));

			//setting the offset to xref offset provided
			pdf_seek(doc->file, entry->offsets, PDFSEEK_SET);

			printf("location = %d and len=%d\n", entry->offsets, lenp);
			stream=pdf_inflate(doc->file, lenp);
			if ( stream ){
				printf("Got stream %s\n", stream);
			}else{
				printf("Not have a stream\n");
				exit(0);
			}
		}
	}

	C_MODE_END
