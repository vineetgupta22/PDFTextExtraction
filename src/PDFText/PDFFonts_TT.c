/**
*	@file		:	PDFFonts_TT.c
*	@brief		:	Parser TrueType Font
**/

	#include <PDFTextExtraction.h>

	C_MODE_START


	/***************************** Starting Prototypes ********************/
	void pdf_load_font_tt(pdf_document *doc, pdf_obj *obj);
	/***************************** Ending Prototypes **********************/

	/***************************** Global Variables ********************/
	/***************************** Global Variables ********************/

	void pdf_load_font_tt(pdf_document *doc, pdf_obj *obj){
		pdf_obj *descriptor;

		//Getting the font descriptor if available
		descriptor = pdf_dict_gets(doc, obj, "FontDescriptor");

		if (descriptor){
			pdf_obj *obj1, *obj2, *obj3, *obj4;

			obj1 = pdf_dict_gets(doc, descriptor, "FontFile");
			obj2 = pdf_dict_gets(doc, descriptor, "FontFile2");
			obj3 = pdf_dict_gets(doc, descriptor, "FontFile3");
			obj4 = obj1 ? obj1 : obj2 ? obj2 : obj3;

			if ( obj4 ) {
				if ( obj4->kind == PDF_INDIRECT){

				}
			}else{
				printf("Font File is not Avilable\n");
				exit(0);
			}
		}else{
			printf("Font descriptor not available\n");
			exit(0);
		}
	}

	C_MODE_END
