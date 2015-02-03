/**
*	@file		:	PDFIndirect.c
*	@brief		:	For doing things about Indirect Objects
**/

	#include <PDFTextExtraction.h>

	C_MODE_START


	/***************************** Starting Prototypes ********************/
	pdf_obj *pdf_array_get(pdf_obj *obj, int i);
	pdf_obj *pdf_dict_get_val(pdf_obj *obj, int i);
	void pdf_set_obj_parent(pdf_obj *obj, int num);
	pdf_xref_entry *pdf_get_xref_entry(pdf_document *doc, int i);
	pdf_obj *pdf_resolve_indirect(pdf_document *doc, pdf_obj *ref);
	pdf_xref_entry *pdf_cache_object(pdf_document *doc, int num, int gen);
	/***************************** Ending Prototypes **********************/

	/***************************** Global Variables ********************/
	/***************************** Global Variables ********************/
	/* Used after loading a document to access entries */
	pdf_xref_entry *pdf_get_xref_entry(pdf_document *doc, int i){
		int j;
		pdf_xref *xref;
		pdf_xref_subsec *sub=NULL;

		if (i < 0){
			printf("Negative object number requested\n");
			exit(0);
		}

		if (i <= doc->max_xref_len){
			j = doc->xref_index[i];
		}else{
			j=0;
		}

		printf("object number %d location in xref_section[%d]\n", i, j);

		/* Find the first xref section where the entry is defined. */
		for (; j < doc->total_xref_sections; j++){
			xref = &doc->xref_sections[j];

			if (i < xref->objects){
				//Searching sections
				for (sub = xref->subsec; sub != NULL; sub = sub->next){
					pdf_xref_entry *entry;
					if (i < sub->start || i >= sub->start + sub->len){
						continue;
					}

					entry = &sub->table[i - sub->start];
					if (entry->type){
						doc->xref_index[i] = j;
						return entry;
					}
				}
			}
		}
		return &sub->table[i - sub->start];
	}

	pdf_obj *pdf_dict_get_val(pdf_obj *obj, int i){
		do {
			if (obj && obj->kind == PDF_INDIRECT){
				printf("obj = pdf_resolve_indirect(obj);\n");
				exit(0);
			}
		} while (0);
		if (!obj || obj->kind != PDF_DICT)
			return NULL;

		if (i < 0 || i >= obj->u.d.len)
			return NULL;
		return obj->u.d.items[i].v;
	}

	pdf_obj *pdf_array_get(pdf_obj *obj, int i){
		do {
			if (obj && obj->kind == PDF_INDIRECT){
				printf("obj = pdf_resolve_indirect(obj);\n");
				exit(0);
			}
		} while (0);

		if (!obj || obj->kind != PDF_ARRAY)
			return NULL;

		if (i < 0 || i >= obj->u.a.len)
			return NULL;

		return obj->u.a.items[i];
	}


	void pdf_set_obj_parent(pdf_obj *obj, int num){
		if (!obj){
			printf("no object set\n");
			return;
		}

		int n, i;
		obj->parent_num = num;

		switch(obj->kind){
			case PDF_ARRAY:
				n=obj->u.a.len;
				for (i = 0; i < n; i++){
					pdf_set_obj_parent(pdf_array_get(obj, i), num);
				}
				break;
			case PDF_DICT:
				n=obj->u.d.len;
				for (i = 0; i < n; i++){
					pdf_set_obj_parent(pdf_dict_get_val(obj, i), num);
				}
				break;
		}
	}


	pdf_xref_entry *pdf_cache_object(pdf_document *doc, int num, int gen PDFUnused){
		pdf_xref_entry *x;
		PDFUnused int rnum, rgen;

		x = pdf_get_xref_entry(doc, num);

		if (x->obj != NULL){
			return x;
		}

		rnum = num;
		if (x->type == 'f'){
			printf("object is free\n");
			exit(0);
		}else if (x->type == 'n'){
			//Setting the offsets
			pdf_seek(doc->file, x->offsets, PDFSEEK_SET);

			x->obj = pdf_parse_ind_obj(doc, doc->file, &doc->lexbuf.base, &rnum, &rgen, &x->offsets);
		}else if (x->type == 'o'){
			printf("object type = o\n");
			exit(0);
		}else{
			printf("else\n");
			exit(0);
		}

		pdf_set_obj_parent(x->obj, num);
		return x;
	}


	pdf_obj *pdf_resolve_indirect(pdf_document *doc PDFUnused, pdf_obj *ref){
		int sanity = 10;
		int num;
		int gen;
		pdf_xref_entry *entry;

		while (ref->kind == PDF_INDIRECT){
			if (--sanity == 0){
				printf("too many indirections (possible indirection cycle involving\n");
				exit(0);
			}

			num = ref->u.r.num;
			gen = ref->u.r.gen;

			if (num <= 0 || gen < 0){
				printf("invalid indirect reference (%d %d R)", num, gen);
				exit(0);
			}

			//Sending get the entry
			entry = pdf_cache_object(doc, num, gen);

			if (entry->obj == NULL){
				printf("entry->obj == NULL\n");
				exit(0);
			}

			ref = entry->obj;
		}
		return ref;
	}


	C_MODE_END
