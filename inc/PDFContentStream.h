/**
*	@file		:	PDFContentStream.h
*	@brief		:	Contents Stream used in Project
**/

	#ifndef		_PDFContentStream_
		/**
		*	@def		_PDFContentStream_
		*	@brief		Header Including Parameter for Only Once.
		**/
		#define			_PDFContentStream_

		//doing alignment of 4 bytes as required by some systems
		#pragma		pack(push, 4)

		typedef 	struct PDFTextExtraction_Documents_Content_Stream					pdf_contents;
		typedef 	struct PDFTextExtraction_Documents_Content_Stream_Stack				pdf_contents_stack;
		typedef		struct PDFTextExtraction_Documents_Content_Stream_Lines				pdf_content_line;
		typedef 	struct PDFTextExtraction_Documents_Content_Stream_Lines_Fonts		pdf_content_fonts;
		typedef 	struct PDFTextExtraction_Documents_Content_Stream_Line_Font			pdf_content_line_font;

		struct PDFTextExtraction_Documents_Content_Stream_Line_Font{
			/**
			*	@var					prev
			*	@brief					Linking to Previous Line
			**/
			char 						fontName[32];

			/**
			*	@var					type
			*	@brief					Type of Font True Type/Type1 etc.
			**/
			char						type[16];

			/**
			*	@var					is_italic
			*	@brief					Boolean for font if italic
			**/
			int							is_italic;

			/**
			*	@var					is_bold
			*	@brief					Boolean for font if bold
			**/
			int							is_bold;

			/**
			*	@var					Size
			*	@brief					size of Font used in Line
			**/
			int							size;
		};

		struct PDFTextExtraction_Documents_Content_Stream_Lines{
			/**
			*	@var					prev
			*	@brief					Linking to Previous Line
			**/
			pdf_content_line			*prev;

			/**
			*	@var					LineNumber
			*	@brief					Current Line Number
			**/
			int							LineNumber;

			/**
			*	@var					PartNumber
			*	@brief					Current Part Number of Line
			**/
			int							PartNumber;

			/**
			*	@var					len
			*	@brief					Length of Text we have read
			**/
			int							len;

			/**
			*	@var					text
			*	@brief					Text of PDF Line
			**/
			char						text[256];

			/**
			*	@var		:			offset_x
			*	@brief		:			Offset of x axis where line starts
			**/
			float						offset_x;

			/**
			*	@var		:			offset_y
			*	@brief		:			Offset of y axis where line starts
			**/
			float						offset_y;

			/**
			*	@var		:			scaler_y
			*	@brief		:			Scaling y units
			**/
			float						scaler_x;

			/**
			*	@var		:			scaler_y
			*	@brief		:			Scaling y units
			**/
			float						scaler_y;

			/**
			*	@var		:			tan_a
			*	@brief		:			Skew tan a
			**/
			float						tan_a;

			/**
			*	@var		:			tan_b
			*	@brief		:			Skew tan b
			**/
			float						tan_b;

			/**
			*	@var		:			maxheight
			*	@brief		:			Maximum Height of Line text
			**/
			float						maxheight;

			/**
			*	@var		:			maxheight
			*	@brief		:			Maximum Height of Line text
			**/
			float						Height_Offset_Y;

			/**
			*	@var		:			maxheight
			*	@brief		:			Maximum Height of Line text
			**/
			float						Text_Leading;

			/**
			*	@var					font
			*	@brief					Font Details of Line used by PDF File
			**/
			pdf_content_line_font		*font;

			/**
			*	@var					next
			*	@brief					Linking to the Next Line
			**/
			pdf_content_line			*next;
		};

		struct PDFTextExtraction_Documents_Content_Stream_Lines_Fonts{
			/**
			*	@var					ref_name
			*	@brief					Reference Name given found in PDF File
			**/
			char						ref_name[8];

			/**
			*	@var					name
			*	@brief					Name of the Font given by Company Created it
			**/
			char						name[32];

			/**
			*	@var					type
			*	@brief					Type of Font True Type/Type1 etc.
			**/
			char 						type[16];

			/**
			*	@var					FirstChar
			*	@brief					First Character which is defined by Font
			**/
			int							FirstChar;

			/**
			*	@var					LastChar
			*	@brief					Last Character Number described by font
			**/
			int							LastChar;

			/**
			*	@var					Width[256]
			*	@brief					Width of Characters used in font
			**/
			int							Width[256];

			/**
			*	@var					Ascent
			*	@brief					Height above used by Font
			**/
			int							Ascent;

			/**
			*	@var					CapHeight
			*	@brief					Height of Capital Letter Character
			**/
			int							CapHeight;

			/**
			*	@var					Descent
			*	@brief					Descent Height used by Font
			**/
			int							Descent;

			/**
			*	@var					ItalicAngle
			*	@brief					Font Italic Angle
			**/
			int							ItalicAngle;

			/**
			*	@var					XHeight
			*	@brief					Maximum Height of X character
			**/
			int							XHeight;

			/**
			*	@var					is_italic
			*	@brief					Boolean for font if italic
			**/
			int							is_italic;

			/**
			*	@var					is_bold
			*	@brief					Boolean for font if bold
			**/
			int							is_bold;

			/**
			*	@var					next
			*	@brief					Reference to Next font details
			**/
			pdf_content_fonts			*next;
		};

		struct PDFTextExtraction_Documents_Content_Stream_Stack{
			/**
			*	@var					obj
			*	@brief					While parsing text, needed to store any object
			**/
			pdf_obj 					*obj;

			/**
			*	@var					name
			*	@brief					While parsing text for storing name token
			**/
			char 						name[256];

			/**
			*	@var					string
			*	@brief					While parsing text for storing string
			**/
			unsigned char 				string[256];

			/**
			*	@var					string_len
			*	@brief					While parsing text for storing string length used
			**/
			int 						string_len;

			/**
			*	@var					stack
			*	@brief					While parsing text for Numbers in stack
			**/
			float 						stack[32];

			/**
			*	@var					top
			*	@brief					Stack tracking of filled
			**/
			int 						top;

			/**
			*	@var					xbalance
			*	@brief					For tracking balance of tex
			**/
			int 						xbalance;

			/**
			*	@var					in_text
			*	@brief					Keeping track of Text Block begins
			**/
			int 						in_text;
		};

		struct PDFTextExtraction_Documents_Content_Stream{
			/**
			*	@var					prev
			*	@brief					Pointer to Previous content stream
			**/
			pdf_contents 				*prev;

			/**
			*	@var					file
			*	@brief					Pointer to PDF File Stream
			**/
			pdf_stream 					*file;

			/**
			*	@var					resources
			*	@brief					Holding resources for Current Page
			**/
			pdf_obj						*resources;

			/**
			*	@var					stack
			*	@brief					Used for holding temporary values
			**/
			pdf_contents_stack			*stack;

			/**
			*	@var					doc
			*	@brief					Holding Document Details
			**/
			pdf_document				*doc;

			/**
			*	@var					PageNo
			*	@brief					Page Number of Current Processing Document
			**/
			int							PageNo;

			/**
			*	@var					TotalLines
			*	@brief					Total Number of Lines Found in Pages
			**/
			int							TotalLines;

			/**
			*	@var					details
			*	@brief					Details of the Lines we have found
			**/
			pdf_content_line			*details;

			/**
			*	@var					allfonts
			*	@brief					Details of All founds found in Page
			**/
			pdf_content_fonts			*allfonts;

			/**
			*	@var					next
			*	@brief					Pointer to Next content stream
			**/
			pdf_contents 				*next;
		};

		//Endling of alignment
		#pragma		pack(pop)
	#endif