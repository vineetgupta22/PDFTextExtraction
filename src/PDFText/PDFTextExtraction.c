/**
*	@file			PDFTextExtraction.c
*	@brief			Library of PDF Text Extraction line by line
**/

	#include <PDFTextExtraction.h>

	C_MODE_START


	/***************************** Starting Prototypes ********************/
	void pdf_info(pdf_document *doc);
	void pdf_file_version(pdf_document *doc);
	void pdf_init_document(pdf_document *doc);
	pdf_document *pdf_new_document(pdf_stream *file);
	void pdf_check_start_overheads(pdf_document *doc);
	PDFExport void PDFAPI PDFTextExtraction(const char *name);
	/***************************** Ending Prototypes **********************/

	/***************************** Global Variables ********************/
	unsigned long MallocSize;
	/***************************** Global Variables ********************/

	/**
	*	@fn			pdf_info(pdf_document *doc)
	*	@param[in]	name	Name of File
	*	@brief		Information Found in PDF File
	**/
	void pdf_info(pdf_document *doc){
		printf("Starting overheads in File \t\t\t= \t%d\n", doc->overhead);
		printf("Version of Document \t\t\t\t= \t%d\n", doc->version);
		if ( doc->binary_data ){
			printf("File contains the Binary Data \t\t\t= \ttrue\n");
		}else{
			printf("File contains the Binary Data \t\t\t= \tfalse\n");
		}
		printf("startxref offset to xref \t\t\t= \t%d\n", doc->startxref);
		printf("\n\nTotal Memory Allocation but not freed\t\t=\t%lu\n\n", MallocSize);
	}

	/**
	*	@fn			void PDFTextExtraction(const char *name)
	*	@param[in]	name	Name of File
	*	@brief		This is the library function which is call to start the
	*	processing of provided file name. It will read entire file Block by
	*	Block and store the extracted Text Line by Line in structure.
	**/
	PDFExport void PDFAPI PDFTextExtraction(const char *name){
		pdf_stream *file = NULL;
		pdf_document *doc;

		//For Keeping the size of malloc done and freed.
		MallocSize=0;

		//Sending to Open the File for Processing
		file = pdf_open_file(name);

		//Init some of structures
		doc = pdf_new_document(file);

		//Started Reading of document
		pdf_init_document(doc);

		//Printing PDF Information
		pdf_info(doc);
	}



	/**
	*	@fn					pdf_new_document(pdf_stream *file)
	*	@param[in]	file	PDF File Stream
	*	@brief		Allocation of Memory for storing the elements of PDF File
	*	which were found on reading.
	**/
	pdf_document *pdf_new_document(pdf_stream *file){
		pdf_document *doc=NULL;

		doc=(pdf_document*)malloc(sizeof(pdf_document));
		memset(doc, 0, sizeof(pdf_document));
		MallocSize+=sizeof(pdf_document);

		//Sending to Init the lex elements for reading document
		pdf_lexbuf_init(&doc->lexbuf.base, PDFTextExt_LEXBUF_LARGE);
		doc->file = pdf_keep_stream(file);

		return doc;
	}


	/**
	*	@fn					pdf_init_document(pdf_document *doc)
	*	@param[in]	doc		PDF Document to be filled
	*	@brief		Starts the reading process of PDF File and fills the
	*	document structure according to the things found in file.
	**/
	void pdf_init_document(pdf_document *doc){
		//Sending to load the xref sections of pdf file
		pdf_load_xref(doc, &doc->lexbuf.base);
	}


	C_MODE_END
