/**
*	@file			PDFStream.c
*	@brief			Functions Relating to Streams
**/

	#include <PDFTextExtraction.h>

	C_MODE_START

	/***************************** Starting Prototypes ********************/
	void pdf_process_stream(pdf_document *doc, pdf_obj *resources, const char *name);
	/***************************** Ending Prototypes **********************/


	/***************************** Global Variables ********************/
	/***************************** Global Variables ********************/

	void pdf_process_stream(pdf_document *doc PDFUnused, pdf_obj *resources PDFUnused, const char *name PDFUnused){
		pdf_contents *contents;

		contents=(pdf_contents*)PDFMalloc(sizeof(*contents));
		memset(contents, 0, sizeof(*contents));

		contents->file=pdf_keep_stream(pdf_open_file(name));

		printf("Breaking process at %d %s\n", __LINE__, __FILE__);
		exit(0);
	}


	C_MODE_END
