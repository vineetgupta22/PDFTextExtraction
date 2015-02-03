/**
*	@file		:	PDFPages.h
*	@brief		:	PDF Pages
**/

	#ifndef		_PDFPages_
		/**
		*	@def		_PDFPages_
		*	@brief		Header Including Parameter for Only Once.
		**/
		#define			_PDFPages_

		//doing alignment of 4 bytes as required by some systems
		#pragma		pack(push, 4)


		typedef			struct PDFTextExtraction_Documents_Object_Pages				pdf_page;
		typedef			struct PDFTextExtraction_Documents_Object_Annot				pdf_annot;
		typedef			struct PDFTextExtraction_Documents_Object_Rect				pdf_rect;

		#define			fz_rect							pdf_rect
		#define			fz_min(a, b) 					(a < b ? a : b)
		#define			fz_max(a, b) 					(a > b ? a : b)


		/**
		*	@brief			pdf_rect is a rectangle represented by two
		*	diagonally opposite corners at arbitrary coordinates.
		*
		*	Rectangles are always axis-aligned with the X- and Y- axes. The
		*	relationship between the coordinates are that x0 <= x1 and y0 <=
		*	y1 in all cases except for infinte rectangles. The area of a rectangle
		*	is defined as (x1 - x0) * (y1 - y0). If either x0 > x1 or y0 > y1
		*	is true for a given rectangle then it is defined to be infinite.
		*	To check for empty or infinite rectangles use pdf_is_empty_rect
		*	and pdf_is_infinite_rect
		*
		*	x0, y0: The top left corner.
		*
		*	x1, y1: The botton right corner.
		*/
		struct PDFTextExtraction_Documents_Object_Rect{
			float 		x0;
			float		y0;
			float 		x1;
			float 		y1;
		};


		struct PDFTextExtraction_Documents_Object_Annot{
			pdf_obj 						*obj;
			int 							ap_iteration;
			pdf_annot 						*next;
			pdf_annot 						*next_changed;
			int 							annot_type;
			int 							widget_type;
		};


		struct PDFTextExtraction_Documents_Object_Pages{
			pdf_rect 					mediabox;
			int 						rotate;
			int 						transparency;
			pdf_obj 					*resources;
			pdf_obj 					*contents;
			pdf_annot 					*annots;
			pdf_annot 					**annot_tailp;
			pdf_annot 					*changed_annots;
			pdf_annot 					*deleted_annots;
			pdf_annot 					*tmp_annots;
			pdf_obj 					*me;
			float 						duration;
			int 						transition_present;
			int 						incomplete;
		};

		//Endling of alignment
		#pragma		pack(pop)
	#endif