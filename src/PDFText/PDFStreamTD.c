/**
*	@file			PDFStreamTD.c
*	@brief			Functions Relating to Streams Parser TD
**/

	#include <PDFTextExtraction.h>

	C_MODE_START

	/***************************** Starting Prototypes ********************/
	void pdf_create_new_line(pdf_contents *contents, int type);
	/***************************** Ending Prototypes **********************/

	/***************************** Global Variables ********************/
	/***************************** Global Variables ********************/

	void pdf_create_new_line(pdf_contents *contents PDFUnused, int type PDFUnused){
		pdf_content_line	*last;
		last=pdf_get_last_content_line(contents);
		if ( last->len == 0 && ! last->prev ){
			return;
		}else{
			printf("Do other things\n");
			exit(0);
		}
	}

	C_MODE_END
