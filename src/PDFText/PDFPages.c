/**
*	@file		:	PDFPages.c
*	@brief		:	For Loading PDF Pages
**/

	#include <PDFTextExtraction.h>

	C_MODE_START


	/***************************** Starting Prototypes ********************/
	void pdf_load_page(pdf_document *doc, int number);
	pdf_obj *pdf_lookup_page_loc(pdf_document *doc, int needle, pdf_obj **parentp, int *indexp);
	pdf_obj *pdf_lookup_page_loc_imp(pdf_document *doc, pdf_obj *node, int *skip, pdf_obj **parentp, int *indexp);
	/***************************** Ending Prototypes **********************/

	/***************************** Global Variables ********************/
	/***************************** Global Variables ********************/

	pdf_obj *pdf_lookup_page_loc_imp(pdf_document *doc, pdf_obj *node, int *skip, pdf_obj **parentp, int *indexp){
		pdf_obj *hit = NULL;
		pdf_obj *kids = NULL;
		int len, i;
		pdf_obj *local_stack[16];
		pdf_obj **stack = &local_stack[0];
		int stack_max = 16;
		int stack_len = 0;

		do
		{
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
				pdf_obj *kid = pdf_array_get(kids, i);
				char *type = pdf_to_name(pdf_dict_gets(doc, kid, "Type"));
				if (*type ? !strcmp(type, "Pages") : pdf_dict_gets(doc, kid, "Kids") && !pdf_dict_gets(doc, kid, "MediaBox")){
					int count = pdf_to_int(pdf_dict_gets(doc, kid, "Count"));
					if (*skip < count){
						node = kid;
						break;
					}else{
						*skip -= count;
					}
				}else{
					if (*type ? strcmp(type, "Page") != 0 : !pdf_dict_gets(doc, kid, "MediaBox")){
						printf("non-page object in page tree (%s)\n", type);
						exit(0);
					}
					if (*skip == 0){
						if (parentp) *parentp = node;
						if (indexp) *indexp = i;
						hit = kid;
						break;
					}else{
						(*skip)--;
					}
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
		return hit;
	}

	pdf_obj *pdf_lookup_page_loc(pdf_document *doc, int needle, pdf_obj **parentp, int *indexp){
		pdf_xref *xref = &doc->xref_sections[0];

		//Getting the root object which we already done.
		pdf_obj *root = pdf_dict_gets(doc, xref->trailer, "Root");

		//From root to pages already resolved indirect reference
		pdf_obj *node = pdf_dict_gets(doc, root, "Pages");

		int skip = needle;
		pdf_obj *hit;

		if (!node){
			printf("cannot find page tree\n");
			exit(0);
		}

		hit = pdf_lookup_page_loc_imp(doc, node, &skip, parentp, indexp);

		if ( ! hit ){
			printf("cannot find page %d in page tree\n", needle);
			exit(0);
		}
		return hit;
	}

	void pdf_load_page(pdf_document *doc, int number){
		printf("Reached to load page No.%d\n", number);
		PDFUnused pdf_obj *pageref, *pageobj;

		//First we need to get the page reference object
		pageref = pdf_lookup_page_loc(doc, number, NULL, NULL);

		//Resolving pageobj
		pageobj = pdf_resolve_indirect(doc, pageref);
	}

	C_MODE_END
