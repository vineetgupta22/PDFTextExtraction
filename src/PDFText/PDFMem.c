
	#define MEM_ERROR(str)                                            \
	{                                                                \
		printf("\nMemory free error! At %s line %d\n", file, line);    \
		printf str;                                                    \
		fflush(stdout);                                                \
	}
	
	#include <PDFTextExtraction.h>

	C_MODE_START


	/***************************** Starting Prototypes ********************/
	unsigned int PDFEstimated(unsigned int size);
	PDF_MEM_TRACE *PDFMemTrace(const char *file, unsigned int line, void *ptr, unsigned int size);
	PDFExport void * PDFAPI PDFMemAlloc(unsigned int size, const char *file, unsigned int line);
	PDFExport void PDFAPI PDFMemFree (void *Address, const char *file, int line);
	PDFExport void PDFAPI PDFMemDetails(void);
	/***************************** Ending Prototypes **********************/
	
	
	/** we can use the real ones here! **/
	#undef malloc
	#undef realloc
	#undef free
	
	#if defined(__sparc__) || defined(__mips64)
		/* __sparc__ is not exactly correct because not all sparc machines require 8, but minor difference in memory usage */
		/* __mips64 it to catch IRIX 6.5 */
		#define PointerAlignmentSize 8
	#else
		#define PointerAlignmentSize sizeof(long)
	#endif
	
	
	
	static unsigned int 	PDFTotalCalls=0;
	static unsigned int 	PDFTotalCurrentOverhead=0;
	static unsigned int 	PDFMaxAllowedOverhead=0;
	static unsigned int 	PDFTotalMemAllocated=0;
	static unsigned int 	PDFCurrentEstimated=0;
	static unsigned int 	PDFMemTotalFree=0;
	static unsigned int 	PDFTotalMemMaxAllowed=0;
	static unsigned int 	PDFTotalMaxAllowedEstimated=0;
	static unsigned int 	PDFReallocCalls=0;
	unsigned int 			PDFMemTraceCounter = 0;
	static PDF_MEM_TRACE    TraceData[PDF_MAX_TRACE];
	static PDF_MEM_TRACE    *Free = NULL;
	static int 				last;
	
	
	
	unsigned int PDFEstimated(unsigned int size){
		if (size <= PDF_MINCHUNK){
			return PDF_MINCHUNK;
		}
		return (size + PDF_CHUNKROUNDOFF) & (~PDF_CHUNKROUNDOFF);
	}
	
	PDF_MEM_TRACE *PDFMemTrace(const char *file, unsigned int line, void *ptr, unsigned int size){
		PDF_MEM_TRACE    *Block;
		int            i;
		if (Free){
			Block = Free;
			Free = NULL;
		}else{
			/** $$$ What if we run off the end? **/
			for (i = last; i < PDF_MAX_TRACE; i++){
				if (TraceData[i].File == NULL){
					break;
				}
			}
			
			Block = &TraceData[i];
			last = i + 1;
		}
		// only needed for extra special debugging
		Block->File = file;
		Block->Line = line;
		Block->Ptr = ptr;
		Block->Size = size;
		Block->Count = ++PDFMemTraceCounter;
		
		return Block;
	}
	
	
	
	/* Allocate a chunk of memory */
	PDFExport void * PDFAPI PDFMemAlloc(unsigned int size, const char *file, unsigned int line){
		/* Actual size of memory to be allocated */
		unsigned int        PDFMemSize;
		
		/* Pointer to allocated memory */
		unsigned char 		*PDFMemPtr;
		
		/* Header of memory */
		PDF_MEM_HEADER    	*PDFHeader;
		
		/**
		*	First we have to adjust the memory size for storing information
		*	as debugging, tracking, Statistics of Memmory Allocation and 
		*	freed by Program. 
		**/
		
		PDFMemSize = size + PDF_MEM_OVERHEAD;
		
		/**
		*	Now we have adjusted the actual required and thus we can now 
		*	allocate the memory by call default malloc
		**/
		
		PDFMemPtr = (unsigned char *)malloc(PDFMemSize);
		
		if (PDFMemPtr == NULL){
			printf("At file %s line %d:\n", file, line);
			printf("Ran out of memory (could not Allocate %d more bytes)!", PDFMemSize);
			exit(1);
		}
		/**
		*	We have to keep records of total Memory allocated for Statistics Purposes
		*	We are going to save the size of chunk as first long word of the chunk.
		*	And return the new chunk after writing first long word to the caller
		*	It helps in the tracking of chunk used or not. If used want size of
		*	chunk is used by developer.
		**/
		
		PDFTotalCalls++;
		PDFTotalCurrentOverhead += PDF_MEM_OVERHEAD;
		
		
		if ( PDFTotalCurrentOverhead > PDFMaxAllowedOverhead){
			PDFMaxAllowedOverhead = PDFTotalCurrentOverhead;
		}
		
		PDFTotalMemAllocated +=size;
		PDFCurrentEstimated += PDFEstimated(size);
		
		if (PDFTotalMemAllocated > PDFTotalMemMaxAllowed){
			PDFTotalMemMaxAllowed = PDFTotalMemAllocated;
		}

		if (PDFTotalCurrentOverhead > PDFTotalMaxAllowedEstimated){
			PDFTotalMaxAllowedEstimated = PDFTotalCurrentOverhead;
		}
		
		PDFHeader = (PDF_MEM_HEADER *)PDFMemPtr;
		PDFHeader->Size = size;
		PDFMemPtr = PDFMemPtr + sizeof (PDF_MEM_HEADER);
		
		{
			PDF_MEM_TAIL *Tail;
			
			PDFHeader->Start = PDFMemPtr;

			PDFHeader->Guard1 = PDF_GUARD;

			PDFHeader->Guard2 = PDF_GUARD;

			memset(PDFMemPtr, 0xAA, size);

			Tail = (PDF_MEM_TAIL *)(PDFMemPtr + size);

			Tail->Guard = PDF_GUARD;
		}
		
		PDFHeader->Trace = PDFMemTrace(file, line, PDFMemPtr, size);
		return (PDFMemPtr) ;
	}
	
	
	PDFExport void PDFAPI PDFMemDetails(void){
		printf("Alloc calls: %u, Realloc calls: %u, Free calls: %u\n",  PDFTotalCalls, PDFReallocCalls, PDFMemTotalFree);
		printf("Requested: Maximum usage: %u, Current usage: %u\n",  PDFTotalMemMaxAllowed,  PDFTotalMemAllocated);
		printf("Estimated: Maximum usage: %u, Current usage: %u\n",  PDFTotalMaxAllowedEstimated,  PDFCurrentEstimated);
		
		
		unsigned int    i;
		unsigned ttl = 0;
		for (i = 0; i < PDFTotalCalls; i++){
			if (TraceData[i].File){
				printf("Unfreed: %s line %d: Size: %d Counter: %d\n", 
				TraceData[i].File, TraceData[i].Line, TraceData[i].Size, TraceData[i].Count);
				ttl += TraceData[i].Size;
			}
		}
		printf("Total Unfreed size: %d\n\n", ttl );
		
	}
	
	
	PDFExport void PDFAPI PDFMemFree (void *Address, const char *file, int line){
		PDF_MEM_HEADER    			*PDFHeader;
		unsigned int   	     		PDFMemSize;        /* Size of chunk to free */
		unsigned int     	   		PDFUserSize;
		void        				*PDFMemPtr;
		
		/* ANSI allows free of NULL */
		
		
		if (!Address)
			return;
			
			
		/*
		*  Get the size of the chunk to free from the long word preceding the
		*  address of the chunk.
		*/
		PDFHeader = (PDF_MEM_HEADER *)Address - 1;
		
		{
			PDF_MEM_TAIL *Tail;
			
			if ( (long)Address & ((~(PointerAlignmentSize-1)) != 0) )
				MEM_ERROR(("Address %08X not longword aligned\n", (unsigned int)Address));
				
			if (Address != PDFHeader->Start)
				MEM_ERROR(("Already free: %08X\n", (unsigned int)Address)); 
				// Err_Signal (PWRK$_BUGMEMFREE, 1, Address);
				
			if (PDFHeader->Guard1 != PDF_GUARD)
				MEM_ERROR(("Head Guard 1 overwritten: %08X\n", (unsigned int)&PDFHeader->Guard1));
				
			if (PDFHeader->Guard2 != PDF_GUARD)
				MEM_ERROR(("Head Guard 2 overwritten: %08X\n", (unsigned int)&PDFHeader->Guard2));
				
				
			Tail = (PDF_MEM_TAIL *)((unsigned char *)Address + PDFHeader->Size);
			
			if (Tail->Guard != PDF_GUARD)
				MEM_ERROR(("Tail Guard overwritten: %08X\n", (unsigned int)&Tail->Guard));
        
		}
		
		PDFMemTotalFree++;
		PDFMemPtr = (unsigned char *)PDFHeader;
		PDFUserSize = PDFHeader->Size;
		PDFMemSize = PDFUserSize + PDF_MEM_OVERHEAD;
		
		
		
		
		
		Free = PDFHeader->Trace;
		Free->File = NULL;
		
		
		
		memset (PDFHeader, 0xDD, PDFMemSize);
		
		/* Subtract chunk size from running total */
		
		PDFTotalMemAllocated -= PDFUserSize;
		
		PDFCurrentEstimated -=  PDFEstimated(PDFUserSize);
		
		PDFTotalCurrentOverhead -= PDF_MEM_OVERHEAD;
		
		/* Free the memory */
		free(PDFMemPtr);
	}

	C_MODE_END
