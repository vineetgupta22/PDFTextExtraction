/**
*	@file			PDFStreamLineFont.c
*	@brief			Functions Relating to Streams Line Font Parser
**/

	#include <PDFTextExtraction.h>

	C_MODE_START

	/***************************** Starting Prototypes ********************/
	void pdf_set_line_font(pdf_contents *contents);
	float pdf_set_maxheight(pdf_contents *contents, char *name);
	/***************************** Ending Prototypes **********************/

	/***************************** Global Variables ********************/
	/***************************** Global Variables ********************/

	float pdf_set_maxheight(pdf_contents *contents, char *name){
		float max=0;

		pdf_content_fonts *current, *next;
		for(current=contents->allfonts; current; ){
			next=current->next;
			if ( strcmp(current->name, name) == 0 ){
				//Assent is the maximum height
				max=(float)current->Ascent;

				if ( current->Descent < 0 ){
					max=max-(float)current->Descent;
				}
				break;
			}
			current=next;
		}

		return max/1000;
	}

	void pdf_set_line_font(pdf_contents *contents PDFUnused){
		if ( contents->allfonts ){
			pdf_content_fonts	*current, *next;

			for(current=contents->allfonts; current; ){
				next=current->next;
				if ( strcmp(contents->stack->name, current->ref_name) == 0 ){
					pdf_content_line	*last;
					last=pdf_get_last_content_line(contents);
					if ( last->len ){
						//Sending to Create a New Part of Line
						pdf_create_new_linepart(contents);

						//Re get the last content line
						last=pdf_get_last_content_line(contents);
					}
					if ( last->font ){
						PDFFree(last->font);
					}
					last->font=(pdf_content_line_font*)PDFMalloc(sizeof(pdf_content_line_font));
					memset(last->font, 0, sizeof(pdf_content_line_font));

					pdf_strlcpy(last->font->fontName, current->name, sizeof(last->font->fontName));
					pdf_strlcpy(last->font->type, current->type, sizeof(last->font->type));
					last->font->is_italic=current->is_italic;
					last->font->is_bold=current->is_bold;

					if ( last->font ){
						last->maxheight=(pdf_set_maxheight(contents, last->font->fontName));
					}
					break;
				}
				current=next;
			}
		}
	}

	C_MODE_END
