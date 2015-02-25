/**
*	@file			PDFStreamParser.c
*	@brief			Functions Relating to Streams Parser
**/

	#include <PDFTextExtraction.h>

	C_MODE_START

	/***************************** Starting Prototypes ********************/
	int pdf_run_keyword(pdf_contents *contents, char *buf);
	/***************************** Ending Prototypes **********************/

	/***************************** Global Variables ********************/
	/***************************** Global Variables ********************/

	int pdf_run_keyword(pdf_contents *contents PDFUnused, char *buf PDFUnused){
		int key;

		key = buf[0];
		if (buf[1])	{
			key |= buf[1] << 8;
			if (buf[2]){
				key |= buf[2] << 16;
				if (buf[3]){
					key = 0;
				}
			}
		}

		switch (key){
			case B('c', 's'): break;			//We don't need setcolorspace
			case B('T', 'c'): break;			//We don't need character spacing
			case B('T', 'd'): 					//New Line Settings
				pdf_create_new_line(contents, 0);
				break;
			case B('T', 'f'):					//Settings Relating to Fonts
				pdf_set_line_font(contents);
				break;
			case B('T', 'j'): break;			//We don't need Show text
			case B('T', 'm'):
				pdf_set_text_matrix(contents);	//Setting Text Matrix
				break;
			case B('T', 'w'): break;			//We don't need word spacing
			case B('E', 'T'): return 1;			//Ending the text block
			case B('T', 'J'): break;			//We don't need Show text
			case C('s', 'c', 'n'): break;		//We don't need setcolor
			default:
				printf("Current matching with=%s\n", buf);
				exit(0);
		}
		return 0;
	}

	C_MODE_END
