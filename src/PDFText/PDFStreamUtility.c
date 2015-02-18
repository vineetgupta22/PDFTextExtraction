/**
*	@file			PDFStreamUtility.c
*	@brief			Functions Relating to Streams Utility
**/

	#include <PDFTextExtraction.h>

	C_MODE_START

	/***************************** Starting Prototypes ********************/
	void pdf_clear_stack(pdf_contents *contents);
	void pdf_create_stack(pdf_contents *contents);
	void pdf_create_newLine(pdf_contents *contents);
	void pdf_create_new_linepart(pdf_contents *contents);
	pdf_content_line *pdf_get_last_content_line(pdf_contents *contents);
	/***************************** Ending Prototypes **********************/

	/***************************** Global Variables ********************/
	/***************************** Global Variables ********************/

	void pdf_create_new_linepart(pdf_contents *contents){
		//Get the last line resources before creating new block
		pdf_content_line	*last;
		last=pdf_get_last_content_line(contents);

		pdf_content_line	*line;

		line=(pdf_content_line*)PDFMalloc(sizeof(pdf_content_line));
		memset(line, 0, sizeof(pdf_content_line));

		//Same line number
		line->LineNumber=last->LineNumber;

		//Increase the Part Number of Text Line
		line->PartNumber=last->PartNumber+1;

		line->offset_x=last->offset_x;
		line->offset_y=last->offset_y;

		line->scaler_x=last->scaler_x;
		last->scaler_y=last->scaler_y;

		line->tan_a=last->tan_a;
		last->tan_b=last->tan_b;

		line->maxheight=last->maxheight;
		line->Height_Offset_Y=last->Height_Offset_Y;

		line->font=(pdf_content_line_font*)PDFMalloc(sizeof(pdf_content_line_font));
		memset(line->font, 0, sizeof(pdf_content_line_font));

		//Copying the all the font attributes
		memcpy(line->font, last->font, sizeof(pdf_content_line_font));

		printf("NewPart LineNumber=%03d; PartNumber=%03d; TextLength=%03d; Previous PartNumber=%d\n",
			line->LineNumber, line->PartNumber, line->len, last->PartNumber);

		if ( contents->details ){
			if ( contents->details->next ){
				line->prev=contents->details->prev;
				contents->details->prev->next=line;
				contents->details->prev=line;
			}else{
				line->prev=contents->details;
				contents->details->prev=line;
				contents->details->next=line;
			}
		}else{
			contents->details=line;
		}
	}

	pdf_content_line *pdf_get_last_content_line(pdf_contents *contents){
		pdf_content_line *ret=NULL;

		pdf_content_line	*current, *next;
		for(current=contents->details; current; ){
			next=current->next;
			ret=current;
			current=next;
		}

		return ret;
	}

	void pdf_clear_stack(pdf_contents *contents){
		int i;

		//Drop the contents object
		pdf_drop_obj(contents->stack->obj);
		contents->stack->obj = NULL;
		contents->stack->name[0] = 0;
		contents->stack->string_len = 0;
		for (i = 0; i < contents->stack->top; i++){
			contents->stack->stack[i] = 0;
		}
		contents->stack->top = 0;
	}

	void pdf_create_newLine(pdf_contents *contents){
		pdf_content_line	*line;

		line=(pdf_content_line*)PDFMalloc(sizeof(pdf_content_line));
		memset(line, 0, sizeof(pdf_content_line));

		line->LineNumber=contents->TotalLines++;

		if ( contents->details ){
			if ( contents->details->next ){
				line->prev=contents->details->prev;
				contents->details->prev->next=line;
				contents->details->prev=line;
			}else{
				line->prev=contents->details;
				contents->details->prev=line;
				contents->details->next=line;
			}
		}else{
			contents->details=line;
		}
	}

	void pdf_create_stack(pdf_contents *contents){
		pdf_contents_stack *stack;

		stack=(pdf_contents_stack*)PDFMalloc(sizeof(pdf_contents_stack));
		memset(stack, 0, sizeof(pdf_contents_stack));

		if ( contents->stack ){
			PDFFree(contents->stack);
			contents->stack=stack;
		}else{
			contents->stack=stack;
		}
	}

	C_MODE_END
