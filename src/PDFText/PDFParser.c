/**
*	@file		:	PDFParser.c
*	@brief		:	Parser for PDF File
**/

	#include <PDFTextExtraction.h>

	C_MODE_START


	/***************************** Starting Prototypes ********************/
	pdf_trailer *pdf_parse_dict(pdf_document *doc, pdf_stream *file, pdf_lexbuf *buf);
	/***************************** Ending Prototypes **********************/

	/***************************** Global Variables ********************/
	/***************************** Global Variables ********************/

	pdf_trailer *pdf_parse_dict(pdf_document *doc PDFUnused, pdf_stream *file PDFUnused, pdf_lexbuf *buf PDFUnused){
		pdf_trailer *dict=NULL;

		return dict;
	}

	C_MODE_END
