/**
*	@file			PDFStreamTD.c
*	@brief			Functions Relating to Streams Parser TD
**/

	#include <PDFTextExtraction.h>

	C_MODE_START

	/***************************** Starting Prototypes ********************/
	void pdf_decide_new_line(pdf_contents *contents);
	void pdf_create_new_line(pdf_contents *contents, int type);
	/***************************** Ending Prototypes **********************/

	/***************************** Global Variables ********************/
	/***************************** Global Variables ********************/
	void pdf_decide_new_line(pdf_contents *contents PDFUnused){
		pdf_content_line	*last;
		last=pdf_get_last_content_line(contents);

		if ( last->prev ){
			//Only Checking needed in Case Previous offset x and y are available
			if ( last->prev->offset_x == last->offset_x ){
				printf("Offset X Matched");
				exit(0);
			}else{
				if ( last->prev->offset_y == last->offset_y ){
					//Nothing to do
				}else{
					float diff=last->prev->offset_y-last->offset_y;
					if ( diff > last->scaler_x*last->maxheight && diff > last->scaler_x ){
						last->LineNumber=contents->TotalLines++;
						last->PartNumber=0;
					}else{
						printf("Offset X and Y Not Matched %f", diff);
						exit(0);
					}
				}
			}
		}
	}

	void pdf_create_new_line(pdf_contents *contents PDFUnused, int type PDFUnused){
		pdf_content_line	*last;
		last=pdf_get_last_content_line(contents);
		if ( last->len == 0 && ! last->prev ){
			last->offset_x=contents->stack->stack[0];
			last->offset_y=contents->stack->stack[1];
		}else{
			if ( last->len ){
				pdf_create_new_linepart(contents);

				//Getting the New Created Last Line/Part
				last=pdf_get_last_content_line(contents);
			}
			if ( type ){
				printf("Code Required for TD\n");
				exit(0);
			}else{
				if ( contents->stack->stack[1] == 0 ){
					last->offset_x=(contents->stack->stack[0] * last->prev->scaler_x) +
						(contents->stack->stack[1] * last->prev->tan_b) + last->prev->offset_x;
				}else{
					if ( last->offset_y==contents->stack->stack[1] ) {
						last->offset_x=contents->stack->stack[0];
					}else{
						last->offset_x=contents->stack->stack[0];
						last->offset_y=contents->stack->stack[1];
					}
				}
			}
		}

		if ( last->font ){
			//Re-Calculate
			last->maxheight=(pdf_set_maxheight(contents, last->font->fontName));
			//Setting the Y Axis height Maximum
			last->Height_Offset_Y=pdf_offsety_max(contents);
		}

		pdf_decide_new_line(contents);
	}

	C_MODE_END
