/**
*	@file		:	PDFFonts_TT.c
*	@brief		:	Parser TrueType Font
**/

	#include <PDFTextExtraction.h>

	C_MODE_START


	/***************************** Starting Prototypes ********************/
	int pdf_font_read_short(pdf_stream *file);
	void pdf_load_font_tt(pdf_document *doc, pdf_obj *obj);
	pdf_font *pdf_start_loading_ttf(pdf_document *doc, const char *file);
	/***************************** Ending Prototypes **********************/

	/***************************** Global Variables ********************/
	/***************************** Global Variables ********************/

	int pdf_font_read_short(pdf_stream *file){
		int t=0;

		t=pdf_read_byte(file);
		t=t<<8;
		t |=pdf_read_byte(file);

		return t;
	}

	pdf_font *pdf_start_loading_ttf(pdf_document *doc PDFUnused, const char *file){
		pdf_font *fontfile;

		fontfile=(pdf_font*)PDFMalloc(sizeof(pdf_font));
		memset(fontfile, 0, sizeof(pdf_font));

		//Sending to open font file
		fontfile->file=pdf_keep_stream(pdf_open_file(file));

		fontfile->vmajor=pdf_font_read_short(fontfile->file);

		pdf_free_stream(fontfile->file);
		return fontfile;
	}

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
					pdf_xref_entry *entry;
					entry = pdf_cache_object(doc, obj4->u.r.num, obj4->u.r.gen);

					if ( entry->offsets ){
						int lenp = pdf_to_int(doc, pdf_dict_gets(doc, obj4, "Length"));

						//setting the offset to xref offset provided
						pdf_seek(doc->file, entry->offsets, PDFSEEK_SET);

						printf("location = %d and len=%d\n", entry->offsets, lenp);
						pdf_inflate2(doc->file, lenp);

						pdf_font *fonts;
						fonts=pdf_start_loading_ttf(doc, "font.ttf");

						if ( doc->font ){
							fonts->next=doc->font;
							doc->font=fonts;
						}else{
							doc->font=fonts;
						}
					}
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
