/**
*	@file		:	PDFFonts_Table_OS2.c
*	@brief		:	Parser TrueType O/S2 Font
**/

	#include <PDFTextExtraction.h>

	C_MODE_START


	/***************************** Starting Prototypes ********************/
	pdf_ttc_cmap_format0 *pdf_ttf_read_format0(pdf_stream *file);
	pdf_ttc_cmap_format4 *pdf_ttf_read_format4(pdf_stream *file);
	pdf_ttc_cmap_header *pdf_font_table_cmap_header(pdf_stream *file);
	pdf_ttc_cmap_subtable *pdf_font_cmap_subtables(pdf_stream *file, unsigned n);
	pdf_ttc_cmap *pdf_font_table_cmap(pdf_stream *file, unsigned offset, unsigned length);
	pdf_ttc_cmap_encoding_header *pdf_font_table_cmap_encode_head(pdf_stream *file, unsigned n);
	/***************************** Ending Prototypes **********************/

	/***************************** Global Variables ********************/
	/***************************** Global Variables ********************/

	pdf_ttc_cmap_format4 *pdf_ttf_read_format4(pdf_stream *file){
		pdf_ttc_cmap_format4 *current=NULL;
		unsigned seg_count, i, left, offsets;

		offsets=(unsigned)(pdf_tell(file)-2);

		current=(pdf_ttc_cmap_format4*)PDFMalloc(sizeof(pdf_ttc_cmap_format4));
		memset(current, 0, sizeof(pdf_ttc_cmap_format4));

		current->length=pdf_font_read_ushort(file);
		current->language=pdf_font_read_ushort(file);
		current->seg_count2=pdf_font_read_ushort(file);
		seg_count=current->seg_count2>>1;
		current->search_range=pdf_font_read_ushort(file);
		current->entry_selector=pdf_font_read_ushort(file);
		current->range_shift=pdf_font_read_ushort(file);
		current->end_count=(unsigned int *)PDFMalloc(sizeof(unsigned int) * (seg_count));
		memset(current->end_count, 0, (sizeof(unsigned int) * (seg_count)));
		for(i=0; i<seg_count; i++){
			current->end_count[i]=pdf_font_read_ushort(file);
		}
		current->reserved_pad=pdf_font_read_ushort(file);

		current->start_count=(unsigned int *)PDFMalloc(sizeof(unsigned int) * (seg_count));
		memset(current->start_count, 0, (sizeof(unsigned int) * (seg_count)));
		for(i=0; i<seg_count; i++){
			current->start_count[i]=pdf_font_read_ushort(file);
		}

		current->id_delta=(unsigned int *)PDFMalloc(sizeof(unsigned int) * (seg_count));
		memset(current->id_delta, 0, (sizeof(unsigned int) * (seg_count)));
		for(i=0; i<seg_count; i++){
			current->id_delta[i]=pdf_font_read_ushort(file);
		}

		current->id_range_offset=(unsigned int *)PDFMalloc(sizeof(unsigned int) * (seg_count));
		memset(current->id_range_offset, 0, (sizeof(unsigned int) * (seg_count)));

		for(i=0; i<seg_count; i++){
			current->id_range_offset[i]=pdf_font_read_ushort(file);
		}

		left = (unsigned)(current->length - (unsigned)(pdf_tell(file)-(int)offsets))>>1;

		current->glyph_id_array=(unsigned int *)PDFMalloc(sizeof(unsigned int) * (left));
		memset(current->glyph_id_array, 0, (sizeof(unsigned int) * (left)));

		for(i=0; i<left; i++){
			current->glyph_id_array[i]=pdf_font_read_ushort(file);
		}
		return current;
	}

	pdf_ttc_cmap_format0 *pdf_ttf_read_format0(pdf_stream *file){
		pdf_ttc_cmap_format0 *current=NULL;
		int i;
		current=(pdf_ttc_cmap_format0*)PDFMalloc(sizeof(pdf_ttc_cmap_format0));
		memset(current, 0, sizeof(pdf_ttc_cmap_format0));

		current->length=pdf_font_read_ushort(file);
		current->language=pdf_font_read_ushort(file);

		for(i=0;i<256;i++){
			current->glyphIDArray[i]=(char)pdf_read_byte(file);
		}
		
		return current;
	}

	pdf_ttc_cmap_subtable *pdf_font_cmap_subtables(pdf_stream *file, unsigned n){
		pdf_ttc_cmap_subtable *header=NULL, *current;
		unsigned i;

		for(i=0; i<n; i++){
			current=(pdf_ttc_cmap_subtable*)PDFMalloc(sizeof(pdf_ttc_cmap_subtable));
			memset(current, 0, sizeof(pdf_ttc_cmap_subtable));

			current->format=pdf_font_read_ushort(file);
			if ( current->format == CMAP_FORMAT0 ){
				current->u.f0=pdf_ttf_read_format0(file);
			} else if ( current->format == CMAP_FORMAT4 ){
				current->u.f4=pdf_ttf_read_format4(file);
			}else{
				printf("Kind Implement Format%d\n", current->format);
				exit(0);
			}

			if ( header ){
				current->next=header;
				header=current;
			}else{
				header=current;
			}
		}
		return header;
	}

	pdf_ttc_cmap_encoding_header *pdf_font_table_cmap_encode_head(pdf_stream *file, unsigned n){
		pdf_ttc_cmap_encoding_header *header=NULL, *current;
		unsigned i;

		for(i=0; i<n; i++){
			current=(pdf_ttc_cmap_encoding_header*)PDFMalloc(sizeof(pdf_ttc_cmap_encoding_header));
			memset(current, 0, sizeof(pdf_ttc_cmap_encoding_header));

			current->platform_id=pdf_font_read_ushort(file);
			current->encoding_id=pdf_font_read_ushort(file);
			current->offset=pdf_font_read_ulong(file);

			if ( header ){
				current->next=header;
				header=current;
			}else{
				header=current;
			}
		}
		return header;
	}

	pdf_ttc_cmap_header *pdf_font_table_cmap_header(pdf_stream *file){
		pdf_ttc_cmap_header *head=NULL;

		head=(pdf_ttc_cmap_header*)PDFMalloc(sizeof(pdf_ttc_cmap_header));
		memset(head, 0, sizeof(pdf_ttc_cmap_header));

		head->version=pdf_font_read_ushort(file);
		head->numberSubtables=pdf_font_read_ushort(file);

		return head;
	}

	pdf_ttc_cmap *pdf_font_table_cmap(pdf_stream *file, unsigned offset, unsigned length){
		pdf_ttc_cmap *cmap=NULL;
		int ofs;
		ofs=pdf_tell(file);

		//Setting offsets location
		pdf_seek(file, (int)offset, PDFSEEK_SET);

		cmap=(pdf_ttc_cmap*)PDFMalloc(sizeof(pdf_ttc_cmap));
		memset(cmap, 0, sizeof(pdf_ttc_cmap));

		cmap->header=pdf_font_table_cmap_header(file);
		cmap->encoding_header=pdf_font_table_cmap_encode_head(file, cmap->header->numberSubtables);
		cmap->subtable=pdf_font_cmap_subtables(file, cmap->header->numberSubtables);

		//Check for reading completed or not
		if ( length != ((unsigned)pdf_tell(file)-offset) ){
			printf("Error in CMAP Mapping\n");
			exit(0);
		}

		//Restoring offsets state
		pdf_seek(file, ofs, PDFSEEK_SET);
		return cmap;
	}

	C_MODE_END
