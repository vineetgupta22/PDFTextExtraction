/**
*	@file		:	PDFFonts_Table_OS2.c
*	@brief		:	Parser TrueType O/S2 Font
**/

	#include <PDFTextExtraction.h>

	C_MODE_START


	/***************************** Starting Prototypes ********************/
	pdf_ttc_os2 *pdf_font_table_os2(pdf_stream *file, unsigned offset, unsigned length);
	/***************************** Ending Prototypes **********************/

	/***************************** Global Variables ********************/
	/***************************** Global Variables ********************/

	pdf_ttc_os2 *pdf_font_table_os2(pdf_stream *file, unsigned offset, unsigned length){
		pdf_ttc_os2 *os2=NULL;
		int ofs;
		ofs=pdf_tell(file);

		//Setting offsets location
		pdf_seek(file, (int)offset, PDFSEEK_SET);

		os2=(pdf_ttc_os2*)PDFMalloc(sizeof(pdf_ttc_os2));
		memset(os2, 0, sizeof(pdf_ttc_os2));

		os2->version=pdf_font_read_ushort(file);
		os2->xAvgCharWidth=pdf_font_read_short(file);

		os2->usWeightClass=pdf_font_read_ushort(file);
		os2->usWidthClass=pdf_font_read_ushort(file);
		os2->fsType=pdf_font_read_ushort(file);

		os2->ySubscriptXSize=pdf_font_read_short(file);
		os2->ySubscriptYSize=pdf_font_read_short(file);
		os2->ySubscriptXOffset=pdf_font_read_short(file);
		os2->ySubscriptYOffset=pdf_font_read_short(file);
		os2->ySuperscriptXSize=pdf_font_read_short(file);
		os2->ySuperscriptYSize=pdf_font_read_short(file);
		os2->ySuperscriptXOffset=pdf_font_read_short(file);
		os2->ySuperscriptYOffset=pdf_font_read_short(file);
		os2->yStrikeoutSize=pdf_font_read_short(file);
		os2->yStrikeoutPosition=pdf_font_read_short(file);
		os2->sFamilyClass=pdf_font_read_short(file);
		int i;
		for(i=0; i<10; i++){
			os2->panose[i]=(char)pdf_read_byte(file);
		}

		os2->ulUnicodeRange1=pdf_font_read_ulong(file);
		os2->ulUnicodeRange2=pdf_font_read_ulong(file);
		os2->ulUnicodeRange3=pdf_font_read_ulong(file);
		os2->ulUnicodeRange4=pdf_font_read_ulong(file);

		os2->achVendID[0]=(char)pdf_read_byte(file);
		os2->achVendID[1]=(char)pdf_read_byte(file);
		os2->achVendID[2]=(char)pdf_read_byte(file);
		os2->achVendID[3]=(char)pdf_read_byte(file);

		os2->fsSelection=pdf_font_read_ushort(file);
		os2->usFirstCharIndex=pdf_font_read_ushort(file);
		os2->usLastCharIndex=pdf_font_read_ushort(file);

		os2->sTypoAscender=pdf_font_read_short(file);
		os2->sTypoDescender=pdf_font_read_short(file);
		os2->sTypoLineGap=pdf_font_read_short(file);

		os2->usWinAscent=pdf_font_read_ushort(file);
		os2->usWinDescent=pdf_font_read_ushort(file);
		os2->ulCodePageRange1=pdf_font_read_ulong(file);
		os2->ulCodePageRange2=pdf_font_read_ulong(file);

		//Check for reading completed or not
		if ( length != ((unsigned)pdf_tell(file)-offset) ){
			printf("Error in OS2 Mapping\n");
			exit(0);
		}

		//Restoring offsets state
		pdf_seek(file, ofs, PDFSEEK_SET);
		return os2;
	}

	C_MODE_END
