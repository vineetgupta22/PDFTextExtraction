/**
*	@file		:	PDFPages.c
*	@brief		:	For Loading PDF Pages
**/

	#include <PDFTextExtraction.h>

	C_MODE_START


	/***************************** Starting Prototypes ********************/
	void pdf_load_page(pdf_document *doc, int number);
	pdf_obj *pdf_lookup_inherited_page_item(pdf_document *doc, pdf_obj *node, const char *key);
	pdf_obj *pdf_lookup_page_loc(pdf_document *doc, int needle, pdf_obj **parentp, int *indexp);
	pdf_obj *pdf_lookup_page_loc_imp(pdf_document *doc, pdf_obj *node, int *skip, pdf_obj **parentp, int *indexp);
	/***************************** Ending Prototypes **********************/

	/***************************** Global Variables ********************/
	#define			fz_is_empty_rect(r)				((r)->x0 == (r)->x1 || (r)->y0 == (r)->y1)
	/***************************** Global Variables ********************/


	pdf_obj *pdf_lookup_inherited_page_item(pdf_document *doc, pdf_obj *node, const char *key){
		pdf_obj *node2 = node;
		pdf_obj *val;
		do{
			val = pdf_dict_gets(doc, node, key);
			if (val)
				break;
			if (pdf_mark_obj(doc, node)){
				printf("cycle in page tree (parents)\n");
				exit(0);
			}
			node = pdf_dict_gets(doc, node, "Parent");
		}while (node);

		do{
			pdf_unmark_obj(doc, node2);
			if (node2 == node)
				break;
			node2 = pdf_dict_gets(doc, node2, "Parent");
		}while (node2);
		return val;
	}

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

	void pdf_load_page(pdf_document *doc PDFUnused, int number){
		printf("Reached to load page No.%d\n", number);
		pdf_obj *pageref, *pageobj;
		pdf_obj *obj;
		pdf_page *page;
		float userunit;
		PDFUnused fz_rect mediabox;

		//First we need to get the page reference object
		pageref = pdf_lookup_page_loc(doc, number, NULL, NULL);

		//Resolving pageobj
		pageobj = pdf_resolve_indirect(doc, pageref);

		page = (pdf_page*)PDFMalloc(sizeof(pdf_page));
		memset(page, 0, sizeof(pdf_page));

		page->annot_tailp = &page->annots;
		page->me = pdf_keep_obj(pageobj);

		//Any measurement unit defined in document
		obj = pdf_dict_gets(doc, pageobj, "UserUnit");
		if ( obj ){
			printf("\n\nobj->kind=%c\n\n", obj->kind);
			exit(0);
		}else{
			userunit=1;
		}

		//Getting the Printing Area
		pdf_to_rect(pdf_lookup_inherited_page_item(doc, pageobj, "MediaBox"), &mediabox);
		if (fz_is_empty_rect(&mediabox)){
			printf("cannot find page size for page %d\n", number + 1);
			exit(0);
		}

		page->mediabox.x0 = fz_min(mediabox.x0, mediabox.x1) * userunit;
		page->mediabox.y0 = fz_min(mediabox.y0, mediabox.y1) * userunit;
		page->mediabox.x1 = fz_max(mediabox.x0, mediabox.x1) * userunit;
		page->mediabox.y1 = fz_max(mediabox.y0, mediabox.y1) * userunit;

		//Getting document resources
		page->resources = pdf_lookup_inherited_page_item(doc, pageobj, "Resources");

		//We need to get the font names and types
		if ( page->resources && page->resources->kind == PDF_INDIRECT){
			//need to resolve the indirect object
			obj = pdf_resolve_indirect(doc, page->resources);

			obj = pdf_dict_gets(doc, page->resources, "Font");
			if ( obj && obj->kind == PDF_INDIRECT){
				//Now the obj would be holding all the fonts used in page
				obj=pdf_resolve_indirect(doc, obj);

				//obj is still a dictionary we have to extract the font names
				//from it defining as bold, italics, underline, normal
				//As PDF Document Manual describes each font separately
			}
		}
		pdf_drop_obj(page->me);
		PDFFree(page);
	}

	C_MODE_END
