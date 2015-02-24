/**
*	@file			PDFStreamFont.c
*	@brief			Functions Relating to Streams Font
**/

	#include <PDFTextExtraction.h>

	C_MODE_START

	/***************************** Starting Prototypes ********************/
	pdf_unicode *pdf_unicode2(void);
	void pdf_read_fonts(pdf_contents *contents);
	float pdf_offsety_max(pdf_contents *contents);
	int pdf_font_set( pdf_contents *contents, char *name );
	pdf_unicode *pdf_unicode_settings(pdf_contents *contents, pdf_obj *ToUnicode);
	void pdf_set_font(pdf_contents *contents, char *font_ref_name, pdf_obj *font);
	/***************************** Ending Prototypes **********************/


	/***************************** Global Variables ********************/
	/***************************** Global Variables ********************/

	float pdf_offsety_max(pdf_contents *contents){
		float ret=0;
		//Before proceeding Further lets get the last content line
		pdf_content_line *last=pdf_get_last_content_line(contents);

		if ( last ){
			ret=(pdf_set_maxheight(contents, last->font->fontName)*last->scaler_x);
			ret+=last->offset_y;
			ret+=last->Text_Leading;
		}
		return ret;
	}

	pdf_unicode *pdf_unicode2(void){
		pdf_unicode *unicode=NULL;
		pdf_stream *file = NULL;
		pdf_lexbuf *lex_buffer;

		//Sending to Open the File for Processing
		file = pdf_open_file("ToUnicode");

		lex_buffer=(pdf_lexbuf *)PDFMalloc(sizeof(pdf_lexbuf));
		memset(lex_buffer, 0, sizeof(pdf_lexbuf));

		unicode=(pdf_unicode *)PDFMalloc(sizeof(pdf_unicode));
		memset(unicode, 0, sizeof(pdf_unicode));

		//Sending to Init the lex elements for reading document
		pdf_lexbuf_init(lex_buffer, PDFTextExt_LEXBUF_SMALL);
		pdf_token tok;
		do{
			tok = pdf_lex(file, lex_buffer);
			if ( tok == PDF_TOK_KEYWORD ){
				if ( strcmp(lex_buffer->scratch, "endcodespacerange") == 0 ){

					tok = pdf_lex(file, lex_buffer);
					if (tok == PDF_TOK_INT){
						int t, l, z, i=0;
						int first=0;
						int number;
						t=lex_buffer->i*2;

						//Droping beginbfchar
						tok = pdf_lex(file, lex_buffer);

						for(i=0; i<t; i++){
							tok = pdf_lex(file, lex_buffer);
							if ( tok == PDF_TOK_STRING ){
								if ( first ){
									z=lex_buffer->len;
									for(l=0; l<z; l++){
										if ( ! unicode->Value[number] ){
											if ( lex_buffer->scratch[l] ){
												unicode->Value[number]=lex_buffer->scratch[l];
											}
										}else{
											printf("unicode->Value[number]=%d Already set\n", unicode->Value[number]);
											exit(0);
										}
									}
									first--;
									continue;
								}
								if ( !first ){
									z=lex_buffer->len;
									for(l=0; l<z; l++){
										number=lex_buffer->scratch[l];
									}
									first++;
									continue;
								}
							}else{
								printf("Not PDF_TOK_STRING");
								exit(0);
							}
						}
					}
				}
			}
		}while (tok != PDF_TOK_EOF);

		PDFFree(lex_buffer);
		pdf_free_stream(file);
		return unicode;
	}


	pdf_unicode *pdf_unicode_settings(pdf_contents *contents, pdf_obj *ToUnicode){
		if ( ToUnicode->kind == PDF_INDIRECT ){
			pdf_xref_entry *entry;
			entry = pdf_cache_object(contents->doc, ToUnicode->u.r.num, ToUnicode->u.r.gen);

			if ( entry->offsets ){
				pdf_obj *filters = pdf_dict_gets(contents->doc, ToUnicode, "Filter");

				if ( filters->kind == PDF_NAME ){
					if ( strcmp(pdf_to_name(filters), "FlateDecode" ) == 0 ){
						int lenp = pdf_to_int(contents->doc, pdf_dict_gets(contents->doc, ToUnicode, "Length"));

						//setting the offset to xref offset provided
						pdf_seek(contents->doc->file, entry->offsets, PDFSEEK_SET);

						//Deflating document
						pdf_inflate3(contents->doc->file, lenp, "ToUnicode");

						return pdf_unicode2();
					}
				}else{
					printf("Filters Kind=%c and %d %s\n", filters->kind, __LINE__, __FILE__);
					exit(0);
				}
			}else{
				printf("Doesn't have the offsets %d %s\n", __LINE__, __FILE__);
				exit(0);
			}
		}
		printf("Settings of ToUnicode - Length\n");
		exit(0);
	}

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

				pdf_obj *ToUnicode;
				ToUnicode=pdf_dict_gets(contents->doc, font, "ToUnicode");

				if ( ToUnicode ){
					current->unicode=pdf_unicode_settings(contents, ToUnicode);
				}else{
					printf("Unicode is not available\n");
					exit(0);
				}

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

			for(i=0; i<TotalFonts; i++){
				pdf_obj *font=pdf_dict_get_key(fonts, i);

				if ( ! pdf_font_set( contents, pdf_to_name(font) ) ){
					//Sending for Setting the Font
					pdf_set_font(contents, pdf_to_name(font), pdf_dict_get_val(fonts, i));
				}
			}

		}else{
			printf("Font Resources Not Found\n");
		}
	}

	C_MODE_END
