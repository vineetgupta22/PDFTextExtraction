/**
*	@file		:	PDFFonts.h
*	@brief		:	About fonts
**/

	#ifndef		_PDFFonts_
		/**
		*	@def		_PDFFonts_
		*	@brief		Header Including Parameter for Only Once.
		**/
		#define			_PDFFonts_

		//doing alignment of 4 bytes as required by some systems
		#pragma		pack(push, 4)

		typedef 	struct PDFTextExtraction_Documents_Font								pdf_font;
		typedef 	struct PDFTextExtraction_Documents_Font_Table						pdf_font_table;
		typedef 	struct PDFTextExtraction_Documents_Font_Table_OS2					pdf_ttc_os2;
		typedef 	struct PDFTextExtraction_Documents_Font_Table_CMAP					pdf_ttc_cmap;
		typedef 	struct PDFTextExtraction_Documents_Font_Table_CMAP_Header			pdf_ttc_cmap_header;
		typedef 	struct PDFTextExtraction_Documents_Font_Table_CMAP_Encode_Header	pdf_ttc_cmap_encoding_header;
		typedef 	struct PDFTextExtraction_Documents_Font_Table_CMAP_Subtable			pdf_ttc_cmap_subtable;
		typedef 	struct PDFTextExtraction_Documents_Font_Table_CMAP_Format0			pdf_ttc_cmap_format0;
		typedef 	struct PDFTextExtraction_Documents_Font_Table_CMAP_Format4			pdf_ttc_cmap_format4;


		typedef enum _cmap_platforms_{
			CMAP_UNICODE=0,
			CMAP_MAC=1,
			CMAP_NONE=2,
			CMAP_WIN=3
		}cmap_platforms;


		typedef enum _cmap_subtables_{
			CMAP_FORMAT0=0,
			CMAP_FORMAT2=2,
			CMAP_FORMAT4=4,
			CMAP_FORMAT6=6,
			CMAP_FORMAT8=8,
			CMAP_FORMAT10=10,
			CMAP_FORMAT12=12,
			CMAP_FORMAT13=13,
			CMAP_FORMAT14=14,
		}cmap_subtables;


		struct PDFTextExtraction_Documents_Font_Table_CMAP_Format0{
			unsigned int				length;
			unsigned int				language;
			char						glyphIDArray[256];
		};


		struct PDFTextExtraction_Documents_Font_Table_CMAP_Format4{
			unsigned int				format;
			unsigned int				length;
			unsigned int				language;
			unsigned int				seg_count2;
			unsigned int				search_range;
			unsigned int				entry_selector;
			unsigned int				range_shift;
			unsigned int				*end_count;
			unsigned int				reserved_pad;
			unsigned int				*start_count;
			unsigned int				*id_delta;
			unsigned int				*id_range_offset;
			unsigned int				*glyph_id_array;
		};


		struct PDFTextExtraction_Documents_Font_Table_CMAP_Subtable{
			cmap_subtables				format;
			union{
				pdf_ttc_cmap_format0	*f0;
				pdf_ttc_cmap_format4	*f4;
			}u;
			pdf_ttc_cmap_subtable		*next;
		};


		struct PDFTextExtraction_Documents_Font_Table_CMAP_Encode_Header{
			unsigned int			platform_id;
			unsigned int			encoding_id;
			unsigned int			offset;
			pdf_ttc_cmap_encoding_header	*next;
		};


		struct PDFTextExtraction_Documents_Font_Table_CMAP_Header{
			unsigned int			version;
			unsigned int			numberSubtables;
		};


		struct PDFTextExtraction_Documents_Font_Table_CMAP{
			pdf_ttc_cmap_header				*header;
			pdf_ttc_cmap_encoding_header	*encoding_header;
			pdf_ttc_cmap_subtable			*subtable;
		};


		struct PDFTextExtraction_Documents_Font_Table_OS2{
			/* 0x0001 - more or 0xFFFF */
			unsigned int  		version;
			int   				xAvgCharWidth;
			unsigned int  		usWeightClass;
			unsigned int  		usWidthClass;
			unsigned int		fsType;
			int   				ySubscriptXSize;
			int   				ySubscriptYSize;
			int   				ySubscriptXOffset;
			int   				ySubscriptYOffset;
			int   				ySuperscriptXSize;
			int   				ySuperscriptYSize;
			int   				ySuperscriptXOffset;
			int   				ySuperscriptYOffset;
			int   				yStrikeoutSize;
			int   				yStrikeoutPosition;
			int   				sFamilyClass;

			char    			panose[10];
			unsigned int   		ulUnicodeRange1;        /* Bits 0-31   */
			unsigned int   		ulUnicodeRange2;        /* Bits 32-63  */
			unsigned int   		ulUnicodeRange3;        /* Bits 64-95  */
			unsigned int   		ulUnicodeRange4;        /* Bits 96-127 */

			char    			achVendID[4];
			unsigned int  		fsSelection;
			unsigned int  		usFirstCharIndex;
			unsigned int  		usLastCharIndex;
			int   				sTypoAscender;
			int   				sTypoDescender;
			int   				sTypoLineGap;
			unsigned int  		usWinAscent;
			unsigned int  		usWinDescent;

			/* only version 1 and higher: */
			unsigned int   		ulCodePageRange1;       /* Bits 0-31   */
			unsigned int   		ulCodePageRange2;       /* Bits 32-63  */

			/* only version 2 and higher: */
			int   				sxHeight;
			int   				sCapHeight;
			unsigned int  		usDefaultChar;
			unsigned int  		usBreakChar;
			unsigned int  		usMaxContext;

			/* only version 5 and higher: */
			unsigned int  		usLowerOpticalPointSize;       /* in twips (1/20th points) */
			unsigned int  		usUpperOpticalPointSize;       /* in twips (1/20th points) */
		};


		struct PDFTextExtraction_Documents_Font_Table{
			/*  Name of tale of 4 bytes and 5th to end stream */
			char		name[5];

			/* CheckSum for this table */
			unsigned int			CheckSum;

			/* Offset from beginning of TrueType font file. */
			unsigned int			offset;

			/* Length of this table. */
			unsigned int			length;

			union{
				pdf_ttc_os2			*os2;
				pdf_ttc_cmap		*cmap;
			}u;

			/* Linked list of tables */
			pdf_font_table			*next;
		};


		struct PDFTextExtraction_Documents_Font{
			/* version major */
			int						vmajor;

			/* version minor */
			int						vminor;

			/* Total Number of tables in font */
			int 					numTables;

			/* (Maximum power of 2 <= numTables) x 16 */
			int 					searchRange;

			/* Log2(maximum power of 2 <= numTables). */
			int						entrySelector;

			/* NumTables x 16-searchRange. */
			int						rangeShift;

			pdf_font_table			*tables;

			/* font stream/file */
			pdf_stream				*file;

			/* next font */
			pdf_font				*next;
		};

		//Endling of alignment
		#pragma		pack(pop)
	#endif