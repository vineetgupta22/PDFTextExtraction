/**
*	@file			PDFTextExtraction.c
*	@brief			Library of PDF Text Extraction line by line
**/

	#include <PDFTextExtraction.h>

	C_MODE_START


	/***************************** Starting Prototypes ********************/
	void pdf_info(pdf_document *doc);
	void pdf_cleanup(pdf_document *doc);
	void pdf_file_version(pdf_document *doc);
	void pdf_init_document(pdf_document *doc);
	pdf_document *pdf_new_document(pdf_stream *file);
	void pdf_check_start_overheads(pdf_document *doc);
	PDFExport void PDFAPI PDFTextExtraction(const char *name);
	/***************************** Ending Prototypes **********************/

	/***************************** Global Variables ********************/
	unsigned long MallocSize;
	/***************************** Global Variables ********************/

	void pdf_cleanup(pdf_document *doc PDFUnused){

		pdf_free_stream(doc->file);

		int i;
		for(i=0; i<doc->total_xref_sections; i++){
			pdf_xref *xref = &doc->xref_sections[i];
			if ( xref->trailer != NULL ){
				pdf_drop_obj(xref->trailer);
				pdf_drop_obj(xref->trailer);
			}

			pdf_xref_subsec *sub=xref->subsec, *next;
			while ( sub != NULL ){
				next=sub->next;
				int j;
				for(j=0; j<sub->len; j++){
					if ( sub->table[j].obj != NULL ){
						pdf_drop_obj(sub->table[j].obj);
					}
				}

				PDFFree(sub->table);
				PDFFree(sub);
				sub=next;
			}
		}

		PDFFree(doc->xref_index);
		PDFFree(doc->xref_sections);

		//Cleaning up font section
		pdf_font *current, *next;
		for(current=doc->font; current; ){
			next=current->next;

			//Free font tables
			pdf_font_table *tcurrent, *tnext;
			for(tcurrent=current->tables; tcurrent; ){
				tnext=tcurrent->next;

				if ( strcmp(tcurrent->name, "OS/2") == 0 ){
					if ( tcurrent->u.os2!= NULL ){
						PDFFree(tcurrent->u.os2);
					}
				}else if ( strcmp(tcurrent->name, "cmap") == 0 ){

					pdf_ttc_cmap *cmap=tcurrent->u.cmap;
					if ( cmap!=NULL ){
						if ( cmap->encoding_header != NULL ){
							pdf_ttc_cmap_encoding_header *ecurrent, *enext;
							for(ecurrent=cmap->encoding_header; ecurrent; ){
								enext=ecurrent->next;
								PDFFree(ecurrent);
								ecurrent=enext;
							}
						}

						if ( cmap->subtable != NULL ){
							pdf_ttc_cmap_subtable *scurrent, *snext;
							for(scurrent=cmap->subtable; scurrent; ){
								snext=scurrent->next;
								if ( scurrent->format == CMAP_FORMAT0 ){
									if ( scurrent->u.f0 != NULL ){
										PDFFree(scurrent->u.f0);
									}
								} else if ( scurrent->format == CMAP_FORMAT4 ){
									if ( scurrent->u.f4 != NULL ){
										PDFFree(scurrent->u.f4->end_count);
										PDFFree(scurrent->u.f4->start_count);
										PDFFree(scurrent->u.f4->id_delta);
										PDFFree(scurrent->u.f4->id_range_offset);
										PDFFree(scurrent->u.f4->glyph_id_array);
										PDFFree(scurrent->u.f4);
									}
								}
								PDFFree(scurrent);
								scurrent=snext;
							}
						}

						if ( cmap->header != NULL ){
							PDFFree(cmap->header);
						}
						PDFFree(cmap);
					}
				}
				PDFFree(tcurrent);
				tcurrent=tnext;
			}
			PDFFree(current);
			current=next;
		}

		pdf_pageslables *lhead, *lnext;
		for(lhead=doc->pageslabels; lhead; ){
			lnext=lhead->next;
			PDFFree(lhead->kind);
			PDFFree(lhead);
			lhead=lnext;
		}

		pdf_contents *head, *ccurr;
		for(head=doc->contents; head; ){
			ccurr=head->next;
			if(head->stack){
				PDFFree(head->stack);
			}
			if ( head->details ){
				pdf_content_line *cline, *cnext;
				for(cline=head->details; cline; ){
					cnext=cline->next;
					PDFFree(cline);
					cline=cnext;
				}
			}
			PDFFree(head);
			head=ccurr;
		}

		PDFFree(doc);

		printf("\n\n\n");
		//Starting of clean up process
		PDFMemDetails();
	}

	/**
	*	@fn			pdf_info(pdf_document *doc)
	*	@param[in]	name	Name of File
	*	@brief		Information Found in PDF File
	**/
	void pdf_info(pdf_document *doc){
		//Starting of clean up process

		printf("Starting overheads in File \t\t\t= \t%d\n", doc->overhead);
		printf("Version of Document \t\t\t\t= \t%d\n", doc->version);
		if ( doc->binary_data ){
			printf("File contains the Binary Data \t\t\t= \ttrue\n");
		}else{
			printf("File contains the Binary Data \t\t\t= \tfalse\n");
		}
		printf("startxref offset to xref \t\t\t= \t%d\n", doc->startxref);
		printf("Total xref sections found in document \t\t=\t%d\n", doc->total_xref_sections);
		printf("Xref Sub Sections Length found in document \t=\t%d\n", doc->max_xref_len);
		
		int j;
		
		for(j=0; j<doc->total_xref_sections; j++){
			pdf_xref *xref = &doc->xref_sections[j];
			pdf_xref_subsec *sub=xref->subsec;
			int f=sub->len;
			printf("Xref SubSection[%d] Offsets filled in document \t=\t%d\n", j, f);
		}

		printf("Total Number of Pages Found in Root=>Catalogue \t=\t%d\n", doc->page_count);
		
		pdf_cleanup(doc);
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

		doc=(pdf_document*)PDFMalloc(sizeof(pdf_document));
		memset(doc, 0, sizeof(pdf_document));

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

		//The rough estimate of pages we already know now its time to
		//load page one by one
		/**
		*	Note: In PDF Page Numbers starts from 0
		**/
		int i;
		for(i=0; i<doc->page_count; i++){
			pdf_load_page(doc, i);
			if ( i==4) break;
		}
	}


	C_MODE_END
