/**
*	@file		:	PDFObjects.c
*	@brief		:	PDF Object Functions
**/

	#include <PDFTextExtraction.h>

	C_MODE_START


	/***************************** Starting Prototypes ********************/
	pdf_obj *pdf_new_dict(void);
	pdf_obj *pdf_new_bool(int b);
	pdf_obj *pdf_new_int(int i);
	int pdf_to_int(pdf_obj *obj);
	void pdf_drop_obj(pdf_obj *obj);
	void pdf_free_dict(pdf_obj *obj);
	void pdf_free_array(pdf_obj *obj);
	char *pdf_to_name(pdf_obj *obj);
	void pdf_free_trailer(pdf_obj *obj);
	pdf_obj *pdf_keep_obj(pdf_obj *obj);
	pdf_obj *pdf_new_name(const char *str);
	pdf_obj *pdf_new_indirect(int num, int gen);
	int pdf_find_obj(pdf_obj *obj, const char *key);
	pdf_obj *pdf_new_string(const char *str, int len);
	pdf_obj *pdf_dict_gets(pdf_obj *obj, const char *key);
	int pdf_dict_finds(pdf_obj *obj, const char *key, int *location);
	void object_altered(pdf_document *doc, pdf_obj *obj, pdf_obj *val);
	void pdf_array_push(pdf_document *doc, pdf_obj *obj, pdf_obj *item);
	void pdf_dict_put(pdf_document *doc, pdf_obj *obj, pdf_obj *key, pdf_obj *val);
	/***************************** Ending Prototypes **********************/

	/***************************** Global Variables ********************/
	/***************************** Global Variables ********************/

	pdf_obj *pdf_new_bool(int b){
		pdf_obj *obj;

		obj =(pdf_obj*)PDFMalloc(sizeof(pdf_obj));
		memset(obj, 0, sizeof(pdf_obj));

		obj->refs = 1;
		obj->kind = PDF_BOOL;
		obj->flags = 0;
		obj->parent_num = 0;
		obj->u.b = b;
		return obj;
	}


	int pdf_to_int(pdf_obj *obj){
		do {
			if (obj && obj->kind == PDF_INDIRECT){
				printf("obj = pdf_resolve_indirect(obj);\n");
				exit(0);
			}
		} while (0);

		if (!obj){
			return 0;
		}
		if (obj->kind == PDF_INT){
			return obj->u.i;
		}
		if (obj->kind == PDF_REAL){
			/* No roundf in MSVC */
			return (int)(obj->u.f + 0.5f);
		}
		return 0;
	}


	pdf_obj *pdf_new_string(const char *str, int len){
		pdf_obj *obj;

		obj =(pdf_obj*)PDFMalloc(sizeof(pdf_obj));
		memset(obj, 0, sizeof(pdf_obj));

		obj->refs = 1;
		obj->kind = PDF_STRING;
		obj->flags = 0;
		obj->parent_num = 0;
		obj->u.s.len = len;

		obj->u.s.buf=(char *)PDFMalloc(10+(unsigned)len);
		memcpy(obj->u.s.buf, str, (unsigned)len);
		obj->u.s.buf[len] = '\0';

		return obj;
	}


	pdf_obj * pdf_new_array(pdf_document *doc PDFUnused, int initialcap){
		pdf_obj *obj;
		int i;

		obj =(pdf_obj*)PDFMalloc(sizeof(pdf_obj));
		memset(obj, 0, sizeof(pdf_obj));

		obj->refs = 1;
		obj->kind = PDF_ARRAY;
		obj->flags = 0;
		obj->parent_num = 0;

		obj->u.a.len = 0;
		obj->u.a.cap = initialcap > 1 ? initialcap : 6;

		obj->u.d.items=(struct keyval*)PDFMalloc(sizeof(struct keyval)*(unsigned)obj->u.d.cap);
		memset(obj->u.d.items, 0, (sizeof(struct keyval)));

		for (i = 0; i < obj->u.a.cap; i++){
			obj->u.a.items[i] = NULL;
		}

		return obj;
	}


	pdf_obj *pdf_new_indirect(int num, int gen){
		pdf_obj *obj=NULL;

		obj =(pdf_obj*)PDFMalloc(sizeof(pdf_obj));
		memset(obj, 0, sizeof(pdf_obj));

		obj->refs = 1;
		obj->kind = PDF_INDIRECT;
		obj->flags = 0;
		obj->parent_num = 0;
		obj->u.r.num = num;
		obj->u.r.gen = gen;

		return obj;
	}
	
	pdf_obj *pdf_new_int(int i){
		pdf_obj *obj=NULL;

		obj =(pdf_obj*)PDFMalloc(sizeof(pdf_obj));
		memset(obj, 0, sizeof(pdf_obj));

		obj->refs = 1;
		obj->kind = PDF_INT;
		obj->flags = 0;
		obj->parent_num = 0;
		obj->u.i = i;

		return obj;
	}

	pdf_obj *pdf_new_name(const char *str){
		pdf_obj *obj=NULL;

		obj =(pdf_obj*)PDFMalloc(sizeof(pdf_obj));
		memset(obj, 0, sizeof(pdf_obj));

		obj->refs = 1;
		obj->kind = PDF_NAME;
		obj->flags = 0;
		obj->parent_num = 0;
		strcpy(obj->u.n, str);

		return obj;
	}

	/**
	*	@fn			pdf_new_dict(void)
	*	@brief		Memory allocation for new pdf object dictionary
	**/
	pdf_obj *pdf_new_dict(void){
		pdf_obj *obj=NULL;

		obj =(pdf_obj*)PDFMalloc(sizeof(pdf_obj));
		memset(obj, 0, sizeof(pdf_obj));

		obj->refs = 1;
		obj->kind = PDF_DICT;
		obj->flags = 0;
		obj->parent_num = 0;

		obj->u.d.len = 0;
		obj->u.d.cap = 10;

		obj->u.d.items=(struct keyval*)PDFMalloc(sizeof(struct keyval)*(unsigned)obj->u.d.cap);
		memset(obj->u.d.items, 0, (sizeof(struct keyval)));

		int i;
		for (i = 0; i < obj->u.d.cap; i++){
			obj->u.d.items[i].k = NULL;
			obj->u.d.items[i].v = NULL;
		}

		return obj;
	}


	char *pdf_to_name(pdf_obj *obj){
		do {
			if (obj && obj->kind == PDF_INDIRECT){
				printf("obj = pdf_resolve_indirect(obj);\n");
				exit(0);
			}
		} while (0);
		if (!obj || obj->kind != PDF_NAME)
			return 0;
		return obj->u.n;
	}


	void pdf_dict_put(pdf_document *doc, pdf_obj *obj, pdf_obj *key, pdf_obj *val){
		do {
			if (obj && obj->kind == PDF_INDIRECT){
				printf("obj = pdf_resolve_indirect(obj);\n");
				exit(0);
			}
		} while (0);

		if (!obj)
			return; 	/* Can't warn :( */

		if (obj->kind != PDF_DICT){
			printf("assert: not a dict\n");
			exit(0);
		}

		do {
			if (key && key->kind == PDF_INDIRECT){
				printf("key = pdf_resolve_indirect(key);\n");
				exit(0);
			}
		} while (0);

		char *s;

		if (key->kind != PDF_NAME){
			printf("assert: key is not a name\n");
			exit(0);
		}else{
			s = pdf_to_name(key);
		}

		if (!val){
			printf("assert: val does not exist for key\n");
			exit(0);
		}

		if (obj->u.d.len > 100 && !(obj->flags & PDF_FLAGS_SORTED)){
			printf("pdf_sort_dict(obj);\n");
			exit(0);
		}

		int i, location;

		i = pdf_dict_finds(obj, s, &location);
		if (i >= 0 && i < obj->u.d.len){
			printf("i>=0\n");
			exit(0);
		}else{
			if ( (unsigned)obj->u.d.len + 1 > (unsigned)obj->u.d.cap){
				printf("pdf_dict_grow(obj);\n");
				exit(0);
			}

			i = location;
			if ((obj->flags & PDF_FLAGS_SORTED) && obj->u.d.len > 0){
				printf("memcopy\n");
				exit(0);
			}
			obj->u.d.items[i].k = pdf_keep_obj(key);
			obj->u.d.items[i].v = pdf_keep_obj(val);
			obj->u.d.len ++;
		}
		object_altered(doc, obj, val);
	}


	void object_altered(pdf_document *doc, pdf_obj *obj, pdf_obj *val PDFUnused){
		if (obj->parent_num == 0 || doc->freeze_updates){
			return;
		}
		printf("otherwise\n");
		exit(0);
	}


	int pdf_dict_finds(pdf_obj *obj, const char *key, int *location){
		//Finding the location of key in dictionary
		if ((obj->flags & PDF_FLAGS_SORTED) && obj->u.d.len > 0){
			printf("PDF_FLAGS_SORTED\n");
			exit(0);
		}else{
			int i;
			for (i = 0; i < obj->u.d.len; i++){
				if (strcmp(pdf_to_name(obj->u.d.items[i].k), key) == 0){
					return i;
				}
			}
			if (location){
				//Key name not found set the len as last location
				*location = obj->u.d.len;
			}
		}
		return -1;
	}

	void pdf_free_dict(pdf_obj *obj){
		int i;

		for (i = 0; i < obj->u.d.len; i++) {
			pdf_drop_obj(obj->u.d.items[i].k);
			pdf_drop_obj(obj->u.d.items[i].v);
		}

		PDFFree(obj->u.d.items);
		PDFFree(obj);
	}

	void pdf_free_array(pdf_obj *obj){
		int i;

		for (i = 0; i < obj->u.a.len; i++){
			pdf_drop_obj(obj->u.a.items[i]);
		}

		PDFFree(obj->u.a.items);
		PDFFree(obj);
	}

	void pdf_drop_obj(pdf_obj *obj){
		if (!obj){
			return;
		}
		if (--obj->refs){
			return;
		}
		if (obj->kind == PDF_ARRAY){
			pdf_free_array(obj);
		}else if (obj->kind == PDF_DICT){
			pdf_free_dict(obj);
		}else if (obj->kind == PDF_STRING){
			PDFFree(obj->u.s.buf);
			PDFFree(obj);
		}else{
			PDFFree(obj);
		}
	}


	pdf_obj *pdf_keep_obj(pdf_obj *obj){
		if (obj)
			obj->refs ++;
		return obj;
	}


	void pdf_array_push(pdf_document *doc, pdf_obj *obj, pdf_obj *item){
		do {
			if (obj && obj->kind == PDF_INDIRECT){
				printf("obj = pdf_resolve_indirect(obj);\n");
				exit(0);
			}
		} while (0);

		if (!obj)
			return; /* Can't warn :( */

		if (obj->kind != PDF_ARRAY){
			printf("assert: not an array\n");
			exit(0);
		}else{
			if ((unsigned)obj->u.a.len + 1 > (unsigned)obj->u.a.cap){
				printf("pdf_array_grow(obj);\n");
				exit(0);
			}
			obj->u.a.items[obj->u.a.len] = pdf_keep_obj(item);
			obj->u.a.len++;
		}

		object_altered(doc, obj, item);
	}


	pdf_obj *pdf_dict_gets(pdf_obj *obj, const char *key){
		do {
			if (obj && obj->kind == PDF_INDIRECT){
				printf("obj = pdf_resolve_indirect(obj);\n");
				exit(0);
			}
		} while (0);

		if (!obj || obj->kind != PDF_DICT)
			return NULL;

		int i;
		i = pdf_dict_finds(obj, key, NULL);
		if (i >= 0)
			return obj->u.d.items[i].v;
		return NULL;
	}


	C_MODE_END
