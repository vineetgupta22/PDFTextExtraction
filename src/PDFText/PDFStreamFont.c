/**
*	@file			PDFStreamFont.c
*	@brief			Functions Relating to Streams Font
**/

	#include <PDFTextExtraction.h>

	C_MODE_START

	/***************************** Starting Prototypes ********************/
	pdf_unicode *pdf_unicode2(void);
	int pdf_encoding_char(char *name);
	void pdf_read_fonts(pdf_contents *contents);
	float pdf_offsety_max(pdf_contents *contents);
	int pdf_font_set( pdf_contents *contents, char *name );
	pdf_encoding *pdf_encoding_details(pdf_contents *contents, pdf_obj *obj);
	pdf_encoding *pdf_encoding_settings(pdf_contents *contents, pdf_obj *Encoding);
	pdf_unicode *pdf_unicode_settings(pdf_contents *contents, pdf_obj *ToUnicode);
	void pdf_set_font(pdf_contents *contents, char *font_ref_name, pdf_obj *font);
	/***************************** Ending Prototypes **********************/


	/***************************** Global Variables ********************/
	#define 			_notdef 			NULL
	const char *pdf_ansi[256] = {
			_notdef,			/* ASCII 000 */
			_notdef,			/* ASCII 001 */
			_notdef,			/* ASCII 002 */
			_notdef,			/* ASCII 003 */
			_notdef,			/* ASCII 004 */
			_notdef,			/* ASCII 005 */
			_notdef,			/* ASCII 006 */
			_notdef,			/* ASCII 007 */
			_notdef,			/* ASCII 008 */
			_notdef,			/* ASCII 009 */
			_notdef,			/* ASCII 010 */
			_notdef,			/* ASCII 011 */
			_notdef,			/* ASCII 012 */
			_notdef,			/* ASCII 013 */
			_notdef,			/* ASCII 014 */
			_notdef,			/* ASCII 015 */
			_notdef,			/* ASCII 016 */
			_notdef,			/* ASCII 017 */
			_notdef,			/* ASCII 018 */
			_notdef,			/* ASCII 019 */
			_notdef,			/* ASCII 020 */
			_notdef,			/* ASCII 021 */
			_notdef,			/* ASCII 022 */
			_notdef,			/* ASCII 023 */
			_notdef,			/* ASCII 024 */
			_notdef,			/* ASCII 025 */
			_notdef,			/* ASCII 026 */
			_notdef,			/* ASCII 027 */
			_notdef,			/* ASCII 028 */
			_notdef,			/* ASCII 029 */
			_notdef,			/* ASCII 030 */
			_notdef,			/* ASCII 031 */
			"space",			/* ASCII 032 */
			"exclam",			/* ASCII 033 */
			"quotedbl",			/* ASCII 034 */
			"numbersign",		/* ASCII 035 */
			"dollar",			/* ASCII 036 */
			"percent",			/* ASCII 037 */
			"ampersand",		/* ASCII 038 */
			"quotesingle",		/* ASCII 039 */
			"parenleft",		/* ASCII 040 */
			"parenright",		/* ASCII 041 */
			"asterisk",			/* ASCII 042 */
			"plus",				/* ASCII 043 */
			"comma",			/* ASCII 044 */
			"hyphen",			/* ASCII 045 */
			"period",			/* ASCII 046 */
			"slash",			/* ASCII 047 */
			"zero",				/* ASCII 048 */
			"one",				/* ASCII 049 */
			"two",				/* ASCII 050 */
			"three",			/* ASCII 051 */
			"four",				/* ASCII 052 */
			"five",				/* ASCII 053 */
			"six",				/* ASCII 054 */
			"seven",			/* ASCII 055 */
			"eight",			/* ASCII 056 */
			"nine",				/* ASCII 057 */
			"colon",			/* ASCII 058 */
			"semicolon",		/* ASCII 059 */
			"less",				/* ASCII 060 */
			"equal",			/* ASCII 061 */
			"greater",			/* ASCII 062 */
			"question",			/* ASCII 063 */
			"at",				/* ASCII 064 */
			"A",				/* ASCII 065 */
			"B",				/* ASCII 066 */
			"C",				/* ASCII 067 */
			"D",				/* ASCII 068 */
			"E",				/* ASCII 069 */
			"F",				/* ASCII 070 */
			"G",				/* ASCII 071 */
			"H",				/* ASCII 072 */
			"I",				/* ASCII 073 */
			"J",				/* ASCII 074 */
			"K",				/* ASCII 075 */
			"L",				/* ASCII 076 */
			"M",				/* ASCII 077 */
			"N",				/* ASCII 078 */
			"O",				/* ASCII 079 */
			"P",				/* ASCII 080 */
			"Q",				/* ASCII 081 */
			"R",				/* ASCII 082 */
			"S",				/* ASCII 083 */
			"T",				/* ASCII 084 */
			"U",				/* ASCII 085 */
			"V",				/* ASCII 086 */
			"W",				/* ASCII 087 */
			"X",				/* ASCII 088 */
			"Y",				/* ASCII 089 */
			"Z",				/* ASCII 090 */
			"bracketleft",		/* ASCII 091 */
			"backslash",		/* ASCII 092 */
			"bracketright",		/* ASCII 093 */
			"asciicircum",		/* ASCII 094 */
			"underscore",		/* ASCII 095 */
			"grave",			/* ASCII 096 */
			"a",				/* ASCII 097 */
			"b",				/* ASCII 098 */
			"c",				/* ASCII 099 */
			"d",				/* ASCII 100 */
			"e",				/* ASCII 101 */
			"f",				/* ASCII 102 */
			"g",				/* ASCII 103 */
			"h",				/* ASCII 104 */
			"i",				/* ASCII 105 */
			"j",				/* ASCII 106 */
			"k",				/* ASCII 107 */
			"l",				/* ASCII 108 */
			"m",				/* ASCII 109 */
			"n",				/* ASCII 110 */
			"o",				/* ASCII 111 */
			"p",				/* ASCII 112 */
			"q",				/* ASCII 113 */
			"r",				/* ASCII 114 */
			"s",				/* ASCII 115 */
			"t",				/* ASCII 116 */
			"u",				/* ASCII 117 */
			"v",				/* ASCII 118 */
			"w",				/* ASCII 119 */
			"x",				/* ASCII 120 */
			"y",				/* ASCII 121 */
			"z",				/* ASCII 122 */
			"braceleft",		/* ASCII 123 */
			"bar",				/* ASCII 124 */
			"braceright",		/* ASCII 125 */
			"asciitilde",		/* ASCII 126 */
			"bullet",			/* ASCII 127 */
			"Euro",				/* ASCII 128 */
			"bullet",			/* ASCII 129 */
			"quotesinglbase",	/* ASCII 130 */
			"florin",			/* ASCII 131 */
			"quotedblbase",		/* ASCII 132 */
			"ellipsis",			/* ASCII 133 */
			"dagger",			/* ASCII 134 */
			"daggerdbl",		/* ASCII 135 */
			"circumflex",		/* ASCII 136 */
			"perthousand",		/* ASCII 137 */
			"Scaron",			/* ASCII 138 */
			"guilsinglleft",	/* ASCII 139 */
			"OE",				/* ASCII 140 */
			"bullet",			/* ASCII 141 */
			"Zcaron",			/* ASCII 142 */
			"bullet",			/* ASCII 143 */
			"bullet",			/* ASCII 144 */
			"quoteleft",		/* ASCII 145 */
			"quoteright",		/* ASCII 146 */
			"quotedblleft",		/* ASCII 147 */
			"quotedblright",	/* ASCII 148 */
			"bullet",			/* ASCII 149 */
			"endash",			/* ASCII 150 */
			"emdash",			/* ASCII 151 */
			"tilde",			/* ASCII 152 */
			"trademark",		/* ASCII 153 */
			"scaron",			/* ASCII 154 */
			"guilsinglright",	/* ASCII 155 */
			"oe",				/* ASCII 156 */
			"bullet",			/* ASCII 157 */
			"zcaron",			/* ASCII 158 */
			"Ydieresis",		/* ASCII 159 */
			"space",			/* ASCII 160 */
			"exclamdown",		/* ASCII 161 */
			"cent",				/* ASCII 162 */
			"sterling",			/* ASCII 163 */
			"currency",			/* ASCII 164 */
			"yen",				/* ASCII 165 */
			"brokenbar",		/* ASCII 166 */
			"section",			/* ASCII 167 */
			"dieresis",			/* ASCII 168 */
			"copyright",		/* ASCII 169 */
			"ordfeminine",		/* ASCII 170 */
			"guillemotleft",	/* ASCII 171 */
			"logicalnot",		/* ASCII 172 */
			"hyphen",			/* ASCII 173 */
			"registered",		/* ASCII 174 */
			"macron",			/* ASCII 175 */
			"degree",			/* ASCII 176 */
			"plusminus",		/* ASCII 177 */
			"twosuperior",		/* ASCII 178 */
			"threesuperior",	/* ASCII 179 */
			"acute",			/* ASCII 180 */
			"mu",				/* ASCII 181 */
			"paragraph",		/* ASCII 182 */
			"periodcentered",	/* ASCII 183 */
			"cedilla",			/* ASCII 184 */
			"onesuperior",		/* ASCII 185 */
			"ordmasculine",		/* ASCII 186 */
			"guillemotright",	/* ASCII 187 */
			"onequarter",		/* ASCII 188 */
			"onehalf",			/* ASCII 189 */
			"threequarters",	/* ASCII 190 */
			"questiondown",		/* ASCII 191 */
			"Agrave",			/* ASCII 192 */
			"Aacute",			/* ASCII 193 */
			"Acircumflex",		/* ASCII 194 */
			"Atilde",			/* ASCII 195 */
			"Adieresis",		/* ASCII 196 */
			"Aring",			/* ASCII 197 */
			"AE",				/* ASCII 198 */
			"Ccedilla",			/* ASCII 199 */
			"Egrave",			/* ASCII 200 */
			"Eacute",			/* ASCII 201 */
			"Ecircumflex",		/* ASCII 202 */
			"Edieresis",		/* ASCII 203 */
			"Igrave",			/* ASCII 204 */
			"Iacute",			/* ASCII 205 */
			"Icircumflex",		/* ASCII 206 */
			"Idieresis",		/* ASCII 207 */
			"Eth",				/* ASCII 208 */
			"Ntilde",			/* ASCII 209 */
			"Ograve",			/* ASCII 210 */
			"Oacute",			/* ASCII 211 */
			"Ocircumflex",		/* ASCII 212 */
			"Otilde",			/* ASCII 213 */
			"Odieresis",		/* ASCII 214 */
			"multiply",			/* ASCII 215 */
			"Oslash",			/* ASCII 216 */
			"Ugrave",			/* ASCII 217 */
			"Uacute",			/* ASCII 218 */
			"Ucircumflex",		/* ASCII 219 */
			"Udieresis",		/* ASCII 210 */
			"Yacute",			/* ASCII 221 */
			"Thorn",			/* ASCII 222 */
			"germandbls",		/* ASCII 223 */
			"agrave",			/* ASCII 224 */
			"aacute",			/* ASCII 225 */
			"acircumflex",		/* ASCII 226 */
			"atilde",			/* ASCII 227 */
			"adieresis",		/* ASCII 228 */
			"aring",			/* ASCII 229 */
			"ae",				/* ASCII 230 */
			"ccedilla",			/* ASCII 231 */
			"egrave",			/* ASCII 232 */
			"eacute",			/* ASCII 233 */
			"ecircumflex",		/* ASCII 234 */
			"edieresis",		/* ASCII 235 */
			"igrave",			/* ASCII 236 */
			"iacute",			/* ASCII 237 */
			"icircumflex",		/* ASCII 238 */
			"idieresis",		/* ASCII 239 */
			"eth",				/* ASCII 240 */
			"ntilde",			/* ASCII 241 */
			"ograve",			/* ASCII 242 */
			"oacute",			/* ASCII 243 */
			"ocircumflex",		/* ASCII 244 */
			"otilde",			/* ASCII 245 */
			"odieresis",		/* ASCII 246 */
			"divide",			/* ASCII 247 */
			"oslash",			/* ASCII 248 */
			"ugrave",			/* ASCII 249 */
			"uacute",			/* ASCII 250 */
			"ucircumflex",		/* ASCII 251 */
			"udieresis",		/* ASCII 252 */
			"yacute",			/* ASCII 253 */
			"thorn",			/* ASCII 254 */
			"ydieresis"			/* ASCII 255 */
		};
	/***************************** Global Variables ********************/

	int pdf_encoding_char(char *name){
		int i;
		int found=0;
		for(i=0; i<256; i++){
			if ( pdf_ansi[i] ){
				if ( strcmp(pdf_ansi[i], name) == 0 ){
					found=1;
					break;
				}
			}
		}
		if ( found == 0 ){
			printf("Not Found Name=%s\n", name);
			exit(0);
		}
		return i;
	}


	pdf_encoding *pdf_encoding_details(pdf_contents *contents PDFUnused, pdf_obj *obj){
		pdf_encoding *encoding=NULL;

		encoding=(pdf_encoding *)PDFMalloc(sizeof(pdf_encoding));
		memset(encoding, 0, sizeof(pdf_encoding));

		int i, len=obj->u.a.len;
		int start=0;

		for(i=0; i<len; i++){
			pdf_obj *temp=obj->u.a.items[i];
			if (temp->kind == PDF_INT){
				start=temp->u.i;
			}else if (temp->kind == PDF_NAME){
				encoding->Value[start++]=pdf_encoding_char(temp->u.n);
			}else{
				printf("Not an Name or Int");
				exit(0);
			}
		}
		return encoding;
	}


	pdf_encoding *pdf_encoding_settings(pdf_contents *contents, pdf_obj *Encoding){
		if ( Encoding->kind == PDF_INDIRECT ){
			Encoding=pdf_resolve_indirect(contents->doc, Encoding);
			if ( Encoding->kind == PDF_DICT ){
				int i, dic_len=Encoding->u.d.len;
				pdf_obj *key, *value;

				for(i=0; i<dic_len; i++){
					key=Encoding->u.d.items[i].k;

					if ( key->kind == PDF_NAME ){
						if ( strcmp (key->u.n, "Differences") == 0 ){
							value=Encoding->u.d.items[i].v;
							if ( value->kind == PDF_ARRAY ){
								return pdf_encoding_details(contents, value);
							}else{
								printf("Differences Not an Array");
								exit(0);
							}
						}
					}else{
						printf("Key Kind is not Name\n");
						exit(0);
					}
				}
			}
		}
		printf("Settings of Encoding - Length\n");
		exit(0);
	}

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
					pdf_obj *Encoding;
					Encoding=pdf_dict_gets(contents->doc, font, "Encoding");
					if ( Encoding ){
						current->encoding=pdf_encoding_settings(contents, Encoding);
					}else{
						printf("Unicode and Encoding not available\n");
						exit(0);
					}
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
