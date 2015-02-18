/**
*	@file			PDFStream.c
*	@brief			Functions Relating to Streams
**/

	#include <PDFTextExtraction.h>

	C_MODE_START

	/***************************** Starting Prototypes ********************/
	void pdf_read_fonts(pdf_contents *contents);
	int pdf_font_set( pdf_contents *contents, char *name );
	void pdf_set_font(pdf_contents *contents, char *font_ref_name, pdf_obj *font);
	/***************************** Ending Prototypes **********************/


	/***************************** Global Variables ********************/
	/***************************** Global Variables ********************/
	
	int pdf_font_set( pdf_contents *contents, char *name){
		int found=0;
		if ( contents->allfonts ){
			pdf_content_fonts	*current, *next;

			for(current=contents->allfonts; current; ){
				next=current->next;
				if ( strcmp(name, current->ref_name) == 0 ){
					found=1;
					break;
				}
				current=next;
			}
		}
		return found;
	}

	void pdf_set_font(pdf_contents *contents, char *font_ref_name, pdf_obj *font){
		if ( font ){
			if ( font->kind == PDF_INDIRECT ){
				font=pdf_resolve_indirect(contents->doc, font);
			}

			if ( font ){
				pdf_content_fonts	*current;

				current=(pdf_content_fonts*)PDFMalloc(sizeof(pdf_content_fonts));
				memset(current, 0, sizeof(pdf_content_fonts));

				//Copy the Reference Name
				pdf_strlcpy(current->ref_name, font_ref_name, sizeof(current->ref_name));
				pdf_strlcpy(current->name,
					pdf_to_name(pdf_dict_gets(contents->doc, font, "BaseFont")),
					sizeof(current->name));
				pdf_strlcpy(current->type,
					pdf_to_name(pdf_dict_gets(contents->doc, font, "Subtype")),
					sizeof(current->type));

				current->FirstChar=pdf_to_int(contents->doc, pdf_dict_gets(contents->doc, font, "FirstChar"));
				current->LastChar=pdf_to_int(contents->doc, pdf_dict_gets(contents->doc, font, "LastChar"));

				int i, found=0, cur=0;
				pdf_obj *widths=pdf_dict_gets(contents->doc, font, "Widths");

				for(i=0; i<256; i++){
					if ( i == current->FirstChar ){
						found=1;
					}
					if ( found ){
						current->Width[i]=pdf_to_int(contents->doc, widths->u.a.items[cur++]);
					}
					if ( i == current->LastChar ){
						found=0;
					}
				}

				printf("FontRefName=%s; FontName=%s; SubType=%s; FirstChar=%d; LastChar=%d;\n",
					current->ref_name, current->name, current->type, current->FirstChar,
					current->LastChar);

				pdf_obj *fontdescriptor=pdf_dict_gets(contents->doc, font, "FontDescriptor");

				if ( fontdescriptor && fontdescriptor->kind == PDF_INDIRECT ){
					//font descriptor
					fontdescriptor=pdf_resolve_indirect(contents->doc, fontdescriptor);
				}
				current->Ascent=pdf_to_int(contents->doc, pdf_dict_gets(contents->doc, fontdescriptor, "Ascent"));
				current->CapHeight=pdf_to_int(contents->doc, pdf_dict_gets(contents->doc, fontdescriptor, "CapHeight"));
				current->ItalicAngle=pdf_to_int(contents->doc, pdf_dict_gets(contents->doc, fontdescriptor, "ItalicAngle"));
				current->XHeight=pdf_to_int(contents->doc, pdf_dict_gets(contents->doc, fontdescriptor, "XHeight"));
				current->Descent=pdf_to_int(contents->doc, pdf_dict_gets(contents->doc, fontdescriptor, "Descent"));

				printf("Ascent=%d; CapHeight=%d; ItalicAngle=%d; XHeight=%d; Descent=%d;\n",
					current->Ascent, current->CapHeight, current->ItalicAngle, current->XHeight,
					current->Descent);

				char *name2;
				found=0;

				name2=strstr(current->name, "Bold");
				if ( name2 ){
					found=1;
					current->is_bold=1;
				}

				name2=strstr(current->name, "Italic");
				if ( name2 ){
					found=1;
					current->is_bold=1;
				}

				if ( !found ){
					if ( current->ItalicAngle ){
						current->is_italic=1;
					}
					pdf_obj *tmp = pdf_dict_gets(contents->doc, fontdescriptor, "FontWeight");
					if ( tmp ){
						printf("FontWeight available\n\n");
						exit(0);
					}
				}

				if ( contents->allfonts ){
					current->next=contents->allfonts;
					contents->allfonts=current;
				}else{
					contents->allfonts=current;
				}
			}
		}
	}

	void pdf_read_fonts(pdf_contents *contents){
		pdf_obj *fonts = pdf_dict_gets(contents->doc, contents->resources, "Font");

		if ( fonts ){
			if ( fonts->kind == PDF_INDIRECT ){
				fonts=pdf_resolve_indirect(contents->doc, fonts);
			}

			int i, TotalFonts=fonts->u.d.len;
			printf("Total Number of Fonts used in Page No.%d=%d\n",
				contents->PageNo, TotalFonts);

			for(i=0; i<TotalFonts; i++){
				printf("Reading Font Number=%d\n", i);
				pdf_obj *font=pdf_dict_get_key(fonts, i);

				if ( ! pdf_font_set( contents, pdf_to_name(font) ) ){
					//Sending for Setting the Font
					pdf_set_font(contents, pdf_to_name(font), pdf_dict_get_val(fonts, i));
				}else{
					printf("Font Name Already set\n");
				}
			}

		}else{
			printf("Font Resources Not Found\n");
		}
	}

	C_MODE_END
