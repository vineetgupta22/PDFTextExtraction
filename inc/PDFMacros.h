/**
*	Copyright (c) 2008-2015, Cee Emm Infotech and/or its affiliates.
*	All rights are reserved.
*
*	Program Name:		PDFTextExtraction
*	Purpose of Program:	Extraction of Text from PDF Files
*	Program Owner:		Cee Emm Infotech
*	Chief Developer:	Vineet Gupta
*	
*	contact: vineetgupta22@gmail.com
*	Address: Cee Emm Infotech, 605, Sector 10D, Chandgiarh [INDIA]
*
**/

/**
*	@file		:	PDFTextExtractionMacros.h
*	@brief		:	Macros used in Project
**/

	#ifndef		_PDFTextExtractionMacros_
		/**
		*	@def	:	_PDFTextExtractionMacros_
		*	@brief	:	Header Including Parameter for Only Once.
		**/
		#define			_PDFTextExtractionMacros_

		//doing alignment of 4 bytes as required by some systems
		#pragma		pack(push, 4)
		
		#ifndef PDFUnused 
			/**
			*	@def		:		PDFUnused
			*	@brief		:		In case Variables are not used in Function then
			*	they must be defined as PDFUnused
			**/
			#define		PDFUnused			__attribute__ ((unused))
		#endif
		
		/**
		*	\if __cplusplus
		*	@def		C_MODE_START
		*	@brief		These days most of the compiler are C++ compilers. To
		*	compiled C Code by C++ Compiler it would generate no error  but effects
		*	linkage. In simple terms we can say:</p><br><p> \li  \ref C_MODE_START /
		*	extern "C" should be defined inside of extern \"C\" blocks. This doesn't
		*	matter, though, since the blocks should nest neatly.</p>
		*
		*			<p> \li \ref C_MODE_START / extern "C" will be defined for
		*	any compilation unit that is being run through the C++ compiler.
		*	When compiling C programs, for the function int  foo(int, int),
		*	the  compiler will generate a symbol in the object file with the
		*	name _foo (for the sake of completeness, the names of the symbols
		*	are compiler dependent). When compiling the same function in C++,
		*	the compiler will generate a name that describes the name of the
		*	function, the return value, the number and type of arguments, and
		*	possibly other things (name space for example). This also means that
		*	functions that are defined as extern "C" cannot be  overloaded.</p>
		*
		*			<p> \li  functions without prototypes will have C++ linkage
		*	if they are in .cpp files and not inside of an extern "C" block. This
		*	is  fine, though, because if it has no prototype, it can only be called
		*	by other functions in the same file, and then you don't generally care
		*	what the linkage looks like, because you aren't planning on having that
		*	function be called by anything outside the same  compilation unit
		*	anyway.</p>
		*
		*			<p> \li Including a header for code that has C linkage (such as
		*	code that was compiled by a C compiler), then you must extern "C" the
		*	header - that way you will be able to link with the library. (Otherwise,
		*	your linker would be looking for functions with names  like _Z1hic when
		*	you were looking for void h(int, char).</p>
		*
		*			<p> \li This sort of mixing is a common reason to use
		*	extern "C",  and I don't see anything wrong with doing it this
		*	way - just make  sure you understand what you are doing.</p>
		*
		*	\endif
		**/

		/**
		*	\if __cplusplus
		*		@def 		C_MODE_END
		*		@brief		The ending of the Nesting \ref C_MODE_START / extern "C".
		*		@see		C_MODE_START
		*	\endif
		**/
		/**
		*	\ifnot __cplusplus
		*		@def	C_MODE_START
		*		@brief	<b>Since the compiler we are using is C and this macro
		*	has no effect i.e. equal to nothing in C Compiler. But the difference
		*	is important to know and which is as under:</b>
		*
		*	These days most of the compiler are C++ compilers. To compiled C Code
		*	by C++ Compiler it would generate no error but effects linkage. In simple
		*	terms we can say:</p>
		*
		*	\li \ref C_MODE_START / extern "C" should be defined inside of extern
		*	\"C\" blocks. This doesn't matter, though, since the blocks should
		*	nest neatly.</p>
		*
		*	\li \ref C_MODE_START / extern "C" will be defined for any compilation
		*	unit that is being run through the C++ compiler. When compiling C
		*	programs, for the function int foo(int, int), the compiler will generate
		*	a symbol in the object file with the name _foo (for the sake of
		*	completeness, the names of the symbols are compiler dependent). When
		*	compiling the same function in C++, the compiler will generate a name
		*	that describes the name of the function, the return value, the number
		*	and type of arguments, and possibly other things (name space for example).
		*	This also means that functions that are defined as extern "C" cannot be
		*	overloaded.</p>
		*
		*	\li  functions without prototypes will have C++ linkage if they are in
		*	.cpp files and not inside of an extern "C" block. This is fine, though,
		*	because if it has no prototype, it can only be called by other functions
		*	in the same file, and then you don't generally care what the linkage
		*	looks like, because you aren't planning on having that function be called
		*	by anything outside the same compilation unit anyway.</p>
		*
		*	\li  Including a header for code that has C linkage
		*	(such as code that was compiled by a C compiler), then you must
		*	extern "C" the header - that way you will be able to link with
		*	the library. (Otherwise, your linker would be looking for
		*	functions with names like _Z1hic when you were looking for
		*	void h(int, char).</p>
		*
		*	\li This sort of mixing is a common reason to use
		*	extern "C", and I don't see anything wrong with doing it this
		*	way - just make sure you understand what you are doing.</p>
		*
		*	\endif
		**/

		/**
		*	\ifnot __cplusplus
		*		@def 	C_MODE_END
		*		@brief	The ending of the Nesting \ref C_MODE_START / extern
		*				"C"
		*		@see	C_MODE_START
		*
		*	\endif
		*
		**/

		#ifdef __cplusplus
			#define C_MODE_START    extern "C" {
			#define C_MODE_END	}
		#else
			#define C_MODE_START
			#define C_MODE_END
		#endif
		
		/**
		*  @def 	NULL
		*
		*  @brief	<b>NULL is very important part of the programming any
		*			language. This is the Standard defination of Null and the
		*			offset() macro used in this project </b></p>
		*
		*	In C++, the null pointer is defined by the ISO specification
		*	(4.10/1) as:
		*
		*	<blockquote>A null pointer constant is an integral constant
		*	expression (5.19) rvalue of integer type that evaluates to zero.
		*	</blockquote>
		*
		*	In C, this rule is similar, but is a bit different (6.3.2.3/3):
		*
		*	<blockquote>An integer constant expression with the value 0,
		*	or such an expression cast to type void *, is called a null pointer
		*	constant). If a null pointer constant is converted to a pointer
		*	type, the resulting pointer, called a null pointer, is guaranteed
		*	to compare unequal to a pointer to any object or function.
		*	</blockquote>
		*
		*	So, <b><i>Should I use NULL or 0</i></b>, when asked to
		*	<b>Bjarne Stroustrup</b> he replied as:</p>
		*
		*	<blockquote>In C++, the definition of NULL is 0, so there is
		*	only an aesthetic difference. I prefer to avoid macros, so I use
		*	0. Another problem with NULL is that people sometimes mistakenly
		*	believe that it is different from 0 and/or not an integer. In
		*	pre-standard code, NULL was/is sometimes defined to something
		*	unsuitable and therefore had/has to be avoided.
		*
		*	That's less common these days.</blockquote>
		*
		*	@see	C_MODE_START To know more differance between C/C++
		**/

		#ifndef NULL
			#ifdef  __cplusplus
				#define NULL    0
			#else
				#define NULL    ((void *)0)
			#endif
		#endif
		
		//Canonical Name of Null
		#ifndef null
			/**
			*	@def				null
			*
			*	@see				NULL
			*/
			#define	null	NULL
		#endif

		#ifndef _PDFBool_
			/**
			*	@def		_PDFBool_
			*	@brief		Own Implementation of Bool
			*/
			#define _PDFBool_
			#undef _Bool

			/**
			*	@typedef		PDFBool
			*	@brief			Own Implementation of Bool
			*/
			typedef signed char PDFBool;
		#endif

		/**
		*	@def	TRUE
		*	@brief	Logical True
		*	@see	PDFBool
		*	@def	true
		*	@brief	Logical True
		*	@see	PDFBool
		*	@def	FALSE
		*	@brief	Logical False
		*	@see	PDFBool
		*	@def	false
		*	@brief	Logical False
		*	@see	PDFBool
		*
		**/
		#ifndef TRUE
			#define TRUE		(PDFBool)1
			#define true		(PDFBool)1
			#define FALSE		(PDFBool)0
			#define false		(PDFBool)0
		#endif
		
		//Also Creating DLL Library for Windows and thus these are required
		#ifdef 		DLL_EXPORT
			#define 	PDFExport 		__declspec(dllexport)
			#define 	PDFImport 		__declspec(dllimport)
			#define 	PDFAPI			__stdcall
		#else
			#define 	PDFExport			/* Nothing */
			#define 	PDFImport			/* Nothing */
			#define 	PDFAPI				/* Nothing */
		#endif

		#ifndef		PDFIsFolder
			/**
			*	@def		:		PDFIsFolder
			*	@brief		:		Checking the Directory Entry is Folder
			**/
			#define		PDFIsFolder			0x4
		#endif

		#ifndef		PDFIsFile
			/**
			*	@def		:		PDFIsFile
			*	@brief		:		Checking the Directory Entry is File or not
			**/
			#define		PDFIsFile			0x8
		#endif


		/**
		*	@def 			PDFSEEK_CUR
		*	@brief			The new file position will be the current
		*		position plus offset
		**/
		#define PDFSEEK_CUR    					1

		/**
		*	@def 			PDFSEEK_END
		*	@brief			The new file position will be the end of the file
		*		plus offset in bytes
		**/
		#define PDFSEEK_END    					2

		/**
		*	@def 			PDFSEEK_SET
		*	@brief			The new file position will be the beginning of the
		*		file plus offset (in bytes).
		**/
		#define PDFSEEK_SET    					0

		/**
		*	@def 			PDFSeek
		*	@brief			Setting the cursor position
		**/
		#define				PDFSeek				lseek
		
		/**
		*	@def 			PDFRead
		*	@brief			PDF File Reading
		**/
		#define				PDFRead				read

		//Endling of alignment
		#pragma		pack(pop)
	#endif