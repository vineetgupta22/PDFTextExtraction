	#ifndef		_PDFMem_
		/**
		*	@def		_PDFMem_
		*	@brief		Header Including Parameter for Only Once.
		**/
		#define			_PDFMem_

		//doing alignment of 4 bytes as required by some systems
		#pragma		pack(push, 4)

		#define 			PDFMalloc(size) 		PDFMemAlloc(size, __FILE__, __LINE__)
		#define 			PDFFree(ptr) 			PDFMemFree(ptr, __FILE__, __LINE__)

		/**
		*	@def			PDF_MINCHUNK
		*	@brief			Minimum size of Chunk allowed to statistics purposes
		**/
		#define 	PDF_MINCHUNK 			16


		/**
		*	@def			PDF_CHUNKROUNDOFF
		*	@brief			Rounding of Chunk size so that it would fit properly
		**/
		#define 	PDF_CHUNKROUNDOFF 		(sizeof(long) - 1)

		/**
		*	@def			PDF_GUARD
		*	@brief			Our Random GAURD value written at beginning and end of
		*	Memory Allocation.
		**/
		#define 	PDF_GUARD 				0x14739182

		/**
		*	@def			PDF_MAX_TRACE
		*	@brief			Maximum Number of Traces tracked.
		**/
		#define 	PDF_MAX_TRACE 			1000000



		/**
		*	@typedef		PDF_MEM_TRACE
		*	@brief			An alias name provided for the struct _PDF_Mem_TraceBLock_
		**/
		typedef		struct _PDF_Mem_TraceBLock_			PDF_MEM_TRACE;



		/**
		*	@struct			_PDF_Mem_TraceBLock_
		*	@brief			Our own Implementation of Memory Track down which block
		*	or variable Memory is not Freed
		**/
		struct _PDF_Mem_TraceBLock_{
			/**
			*	@var					File
			*	@brief					File Name for Track down Memory
			**/
			const char    				*File;

			/**
			*	@var					Line
			*	@brief					Line Number for Track down Memory
			**/
			unsigned int        		Line;

			/**
			*	@var					Size
			*	@brief					Allocated Size for Track down Memory
			**/
			unsigned int 				Size;

			/**
			*	@var					Count
			*	@brief					Memory Allocated on which Number
			**/
			unsigned int 				Count;

			/**
			*	@var					Ptr
			*	@brief					only needed for extra special debugging
			**/
			void    					*Ptr;
		};


		/**
		*	@struct			_PDF_Mem_Tail
		*	@brief			Memory Tail is what is allocated block by the user after
		*	memory allocation
		**/
		typedef struct _PDF_Mem_Tail{
			/**
			*	@var		Guard
			*	@brief		Guard is used for checking purposes
			**/
			unsigned long    Guard;
		} PDF_MEM_TAIL;


		/**
		*	@struct			_PDF_Mem_Header_
		*	@brief			Memory Header is what is stored before allocated block
		**/
		typedef struct _PDF_Mem_Header_{
			/**
			*	@var		Trace
			*	@brief		Pointer to the Trace Memory Block
			**/
			PDF_MEM_TRACE        *Trace;
			unsigned long    	Guard1;
			unsigned int    	Size;
			void            	*Start;
			unsigned long    	Guard2;
		} PDF_MEM_HEADER;

		/**
		*		@def				PDF_MEM_OVERHEAD
		*		@brief				Maximum Memory Overhead in Bytes to be used for Memory
		*			tracking and debugging
		**/
		#define PDF_MEM_OVERHEAD		(sizeof(PDF_MEM_HEADER) + sizeof(PDF_MEM_TAIL))

		//Endling of alignment
		#pragma		pack(pop)
	#endif