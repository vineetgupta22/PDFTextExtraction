/**
*	@file		:	PDFFonts.c
*	@brief		:	Parser for PDF Fonts
**/

	#include <PDFTextExtraction.h>

	C_MODE_START


	/***************************** Starting Prototypes ********************/
	void pdf_font_load(pdf_document *doc, pdf_obj *obj);
	/***************************** Ending Prototypes **********************/

	/***************************** Global Variables ********************/
	/***************************** Global Variables ********************/

	void pdf_font_load(pdf_document *doc, pdf_obj *obj){
		int i, n;
		pdf_obj *font;

		//Now having the only font object
		obj = pdf_dict_gets(doc, obj, "Font");

		//Resolving Font object
		obj = pdf_resolve_indirect(doc, obj);

		//Total Number of fonts used in current document page
		n = obj->u.d.len;

		//Reading each object font
		for(i=0; i<n; i++){
			//Getting the Found object by Reference Number
			font=pdf_dict_get_val(obj, i);

			if ( font->kind == PDF_INDIRECT){
				//Resolving font
				font=pdf_resolve_indirect(doc, font);

				char *subtype;
				subtype = pdf_to_name(pdf_dict_gets(doc, font, "Subtype"));

				if (subtype && !strcmp(subtype, "TrueType")){
					pdf_load_font_tt(doc, font);
				}else{
					printf("Not a true type font\n");
					exit(0);
				}
			}
		}
	}

	C_MODE_END
