/**
*	@file		:	PDFPages.c
*	@brief		:	For Loading PDF Pages
**/

	#include <PDFTextExtraction.h>

	C_MODE_START


	/***************************** Starting Prototypes ********************/
	void pdf_load_pagelabels_nums(pdf_document *doc, pdf_obj *nums);
	void pdf_load_pagelabels(pdf_document *doc, pdf_obj *pagelabels);
	void pdf_set_nums(pdf_document *doc PDFUnused, pdf_obj *nums, int start);
	/***************************** Ending Prototypes **********************/

	/***************************** Global Variables ********************/
	/***************************** Global Variables ********************/

	void pdf_set_nums(pdf_document *doc PDFUnused, pdf_obj *nums, int start){
		int i, len=nums->u.d.len;
		pdf_pageslables *current;

		for(i=0; i<len; i++){
			current=(pdf_pageslables *)PDFMalloc(sizeof(pdf_pageslables));
			memset(current, 0, sizeof(pdf_pageslables));

			if (strcmp(pdf_to_name(nums->u.d.items[i].k), "S") == 0){
				pdf_obj *type=nums->u.d.items[i].v;

				current->kind=(char *)PDFMalloc(8);
				memset(current->kind, 0, 8);

				if ( strcmp(type->u.n, "r") == 0 ){
					strcpy(current->kind, "r");
				}else if ( strcmp(type->u.n, "D") == 0 ){
					strcpy(current->kind, "d");
				}else{
					printf("Kindly provide type = %s\n", type->u.n);
					exit(0);
				}
			}else{
				printf("some things might be wrong\n");
				exit(0);
			}
			current->startfrom=start;
			if ( doc->pageslabels ){
				if ( doc->pageslabels->next ){
					current->next=doc->pageslabels->prev;
					doc->pageslabels->prev=current;
				}else{
					doc->pageslabels->next=current;
					doc->pageslabels->prev=current;
				}
			}else{
				doc->pageslabels=current;
			}
		}
	}

	void pdf_load_pagelabels_nums(pdf_document *doc PDFUnused, pdf_obj *nums){
		int i, len=0;
		if ( nums->kind == PDF_ARRAY ){
			int start_frm=0;
			len=pdf_array_len(nums);
			for(i=0; i<len; i++){
				pdf_obj *obj=pdf_array_get(nums, i);
				if ( obj->kind == PDF_INT ){
					start_frm=obj->u.i;
					i++;
					obj=pdf_array_get(nums, i);
					if ( obj->kind == PDF_INDIRECT ){
						pdf_set_nums(doc, pdf_resolve_indirect(doc, obj), start_frm);
					}else{
						printf("obj->kind=indirect = %c\n", obj->kind);
						exit(0);
					}
				}else{
					printf("obj->kind=indirect = %c\n", obj->kind);
					exit(0);
				}
			}
		}else{
			printf("Kind of PageLabels Nums Object=%c\n", nums->kind);
			exit(0);
		}
	}

	void pdf_load_pagelabels(pdf_document *doc, pdf_obj *pagelabels){
		int i, len=0;
		PDFUnused pdf_obj *objkey, *objval;
		if ( pagelabels->kind == PDF_INDIRECT ){
			pagelabels=pdf_resolve_indirect(doc, pagelabels);

			len=pagelabels->u.d.len;
			for(i=0; i<len; i++){
				//Getting the Key
				objkey=pdf_dict_get_key(pagelabels, i);

				if ( strcmp(pdf_to_name(objkey), "Nums" ) == 0 ){
					pdf_load_pagelabels_nums(doc, pdf_dict_get_val(pagelabels, i));
				}else{
					printf("kindly implement %s object\n\n", pdf_to_name(objkey));
					exit(0);
				}
			}
		}
	}

	C_MODE_END
