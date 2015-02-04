/**
*	@file		:	PDFString.c
*	@brief		:	Functions relating to strings
**/

	#include <PDFTextExtraction.h>

	C_MODE_START

	struct huft {
		unsigned char e;                /* number of extra bits or operation */
		unsigned char b;                /* number of bits in this code or subcode */
		union {
			unsigned short n;              /* literal, length base, or distance base */
			struct huft *t;     /* pointer to next level of table */
		} v;
	};

	/***************************** Starting Prototypes ********************/
	int GETBYTE(void);
	int inflate_block(int *e);
	int inflate_dynamic(void);
	int huft_free(struct huft *t);
	char *pdf_inflate(pdf_stream *file, int length);
	int inflate_codes(struct huft *tl, struct huft *td, int bl, int bd);
	int huft_build(unsigned *b, unsigned n, unsigned s, unsigned short *d, unsigned short *e, struct huft **t, int *m);
	/***************************** Ending Prototypes **********************/
	
	/***************************** Global Variables ********************/
	static pdf_stream *in;
	static int insize;
	static unsigned outcnt;
	unsigned char slide[1UL<<17];
	static unsigned inptr;
	static unsigned long bb;                         /* bit buffer */
	static unsigned bk;                    			/* bits in bit buffer */
	static unsigned hufts;  						/* track memory usage */
	static int lbits = 9;							/* bits in base literal/length lookup table */
	static int dbits = 6;   						/* bits in base distance lookup table */

	/* Copy lengths for literal codes 257..285 */
	static unsigned short cplens[] = {
		3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31,
		35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258, 0, 0
	}; /* note: see note #13 above about the 258 in this list. */

	/* Extra bits for literal codes 257..285 */
	static unsigned short cplext[] = {
		0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2,
		3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0, 99, 99
	}; /* 99==invalid */

	/* Copy offsets for distance codes 0..29 */
	static unsigned short cpdist[] = {
        1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193,
        257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145,
        8193, 12289, 16385, 24577
	};

	/* Extra bits for distance codes */
	static unsigned short cpdext[] = {
		0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6,
		7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13
	};

	/* Order of the bit length code lengths */
	static unsigned border[] = {
		16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 
		2, 14, 1, 15
	};

	static unsigned short mask_bits[] = {
		0x0000, 0x0001, 0x0003, 0x0007, 0x000f, 0x001f, 0x003f, 0x007f, 0x00ff,
		0x01ff, 0x03ff, 0x07ff, 0x0fff, 0x1fff, 0x3fff, 0x7fff, 0xffff
	};

	#define 				window					slide
	#define 				wp 						outcnt
	#define 				NEXTBYTE()  			GETBYTE()
	#define 				NEEDBITS(n) 			{while(k<(n)){b|=((unsigned long)NEXTBYTE())<<k;k+=8;}}
	#define 				DUMPBITS(n) 			{b>>=(n);k-=(n);}
	/* maximum bit length of any code (16 for explode) */
	#define 				BMAX 					16
	/* maximum number of codes in any set */
	#define 				N_MAX 					288

	/***************************** Global Variables ********************/

	int GETBYTE(void){
		if ( insize ){
			return pdf_read_byte(in);
		}else{
			printf("error in bytes\n");
			exit(0);
		}
	}

	int huft_free(struct huft *t){
		register struct huft *p, *q;

		/* Go through linked list, freeing from the malloced (t[-1]) address. */
		p = t;
		while (p != (struct huft *)NULL){
			q = (--p)->v.t;
			free(p);
			p = q;
		}
		return 0;
	}

	/* 	
		Given a list of code lengths and a maximum table size, make a set of
		tables to decode that set of codes.  Return zero on success, one if
		the given code set is incomplete (the tables are still built in this
		case), two if the input is invalid (all zero length codes or an
		oversubscribed set of lengths), and three if not enough memory. 
	*/
	int huft_build(unsigned *b, unsigned n, unsigned s, unsigned short *d, unsigned short *e, struct huft **t, int *m){
		unsigned a;                   /* counter for codes of length k */
		unsigned c[BMAX+1];           /* bit length count table */
		unsigned f;                   /* i repeats in table every f entries */
		int g;                        /* maximum code length */
		int h;                        /* table level */
		register unsigned i;          /* counter, current code */
		register unsigned j;          /* counter */
		register int k;               /* number of bits in current code */
		int l;                        /* bits per table (returned in m) */
		register unsigned *p;         /* pointer into c[], b[], or v[] */
		register struct huft *q;      /* points to current table */
		struct huft r;                /* table entry for structure assignment */
		struct huft *u[BMAX];         /* table stack */
		unsigned v[N_MAX];            /* values in order of bit length */
		register int w;               /* bits before this table == (l * h) */
		unsigned x[BMAX+1];           /* bit offsets, then code stack */
		unsigned *xp;                 /* pointer into x */
		int y;                        /* number of dummy codes added */
		unsigned z;                   /* number of entries in current table */

		/* Generate counts for each bit length */
		memset(c, 0, sizeof(c));

		p = b;  i = n;
		do {
			c[*p]++;                    /* assume all entries <= BMAX */
			p++;                      /* Can't combine with above line (Solaris bug) */
		} while (--i);

		/* null input--all zero length codes */
		if (c[0] == n){
			q = (struct huft *) malloc (3 * sizeof *q);
			if (!q)
				return 3;
			hufts += 3;
			q[0].v.t = (struct huft *) NULL;
			q[1].e = 99;    /* invalid code marker */
			q[1].b = 1;
			q[2].e = 99;    /* invalid code marker */
			q[2].b = 1;
			*t = q + 1;
			*m = 1;
			return 0;
		}

		/* Find minimum and maximum length, bound *m by those */
		l = *m;
		for (j = 1; j <= BMAX; j++){
			if (c[j]){
				break;
			}
		}
		k = (int)j;                        /* minimum code length */
		if ((unsigned)l < j){
			l = (int)j;
		}
		for (i = BMAX; i; i--){
			if (c[i]){
				break;
			}
		}
		g = (int)i;                        /* maximum code length */
		if ((unsigned)l > i){
			l = (int)i;
		}
		*m = l;

		/* Adjust last length count to fill out codes, if needed */
		for (y = 1 << j; j < i; j++, y <<= 1){
			if ((y -= (int)c[j]) < 0){
				return 2;                 /* bad input: more codes than bits */
			}
		}
		if ((y -= (int)c[i]) < 0){
			return 2;
		}
		c[i] += (unsigned)y;

		/* Generate starting offsets into the value table for each length */
		x[1] = j = 0;
		p = c + 1;  xp = x + 2;

		/* note that i == g from above */
		while (--i) {
			*xp++ = (j += *p++);
		}

		/* Make a table of values in order of bit lengths */
		p = b;  i = 0;
		do {
			if ((j = *p++) != 0)
				v[x[j]++] = i;
		} while (++i < n);

		/* set n to length of v */
		n = x[g];

		/* Generate the Huffman codes and for each, make the table entries */
		x[0] = i = 0;                 /* first Huffman code is zero */
		p = v;                        /* grab values in bit order */
		h = -1;                       /* no tables yet--level -1 */
		w = -l;                       /* bits decoded == (l * h) */
		u[0] = (struct huft *)NULL;   /* just to keep compilers happy */
		q = (struct huft *)NULL;      /* ditto */
		z = 0;                        /* ditto */

		/* go through the bit lengths (k already is bits in shortest code) */
		for (; k <= g; k++){
			a = c[k];
			while (a--){
				/* here i is the Huffman code of length k bits for value *p */
				/* make tables up to required level */
				while (k > w + l){
					h++;
					w += l;                 /* previous table always l bits */
					
					/* compute minimum size table less than or equal to l bits */
					z = (z = (unsigned)(g - w)) > (unsigned)l ? (unsigned)l : z;  /* upper limit on table size */
					/* try a k-w bit table */
					if ((f = 1U << (j = (unsigned)(k - w))) > a + 1){
						/* too few codes for k-w bit table */
						/* deduct codes from patterns left */
						f -= a + 1;
						xp = c + k;
						if (j < z){
							/* try smaller tables up to z bits */
							while (++j < z){
								if ((f <<= 1) <= *++xp)
									break;            /* enough codes to use up j bits */
								f -= *xp;           /* else deduct codes from patterns */
							}
						}
					}
					z = 1U << j;             /* table entries for j-bit table */

					/* allocate and link in new table */
					if ((q = (struct huft *)malloc((z + 1)*sizeof(struct huft))) == (struct huft *)NULL){
						if (h)
							huft_free(u[0]);
						return 3;             /* not enough memory */
					}

					hufts += z + 1;         /* track memory usage */
					*t = q + 1;             /* link to list for huft_free() */
					*(t = &(q->v.t)) = (struct huft *)NULL;
					u[h] = ++q;             /* table starts after link */

					/* connect to last table, if there is one */
					if (h){
						x[h] = i;             			/* save pattern for backing up */
						r.b = (unsigned char)l;         /* bits to dump before this table */
						r.e = (unsigned char)(16 + j);  /* bits in this table */
						r.v.t = q;            /* pointer to this table */
						j = i >> (w - l);     /* (get around Turbo C bug) */
						u[h-1][j] = r;        /* connect to last table */
					}
				}

				/* set up table entry in r */
				r.b = (unsigned char)(k - w);
				if (p >= v + n)
					r.e = 99;               /* out of values--invalid code */
				else if (*p < s){
					r.e = (unsigned char)(*p < 256 ? 16 : 15);    /* 256 is end-of-block code */
					r.v.n = (unsigned short)(*p);             /* simple code is just the value */
					p++;                           /* one compiler does not like *p++ */
				}else{
					r.e = (unsigned char)e[*p - s];   /* non-simple--look up in lists */
					r.v.n = d[*p++ - s];
				}

				/* fill code-like entries with r */
				f = 1U << (k - w);
				for (j = i >> w; j < z; j += f)
					q[j] = r;

				/* backwards increment the k-bit code i */
				for (j = 1U << (k - 1); i & j; j >>= 1)
					i ^= j;
				i ^= j;

				/* backup over finished tables */
				while ((i & ((1U << w) - 1)) != x[h]){
					h--;                    /* don't need to update q */
					w -= l;
				}
			}
		}

		/* Return true (1) if we were given an incomplete table */
		return y != 0 && g != 1;
	}

	int inflate_codes(struct huft *tl, struct huft *td, int bl, int bd){
		register unsigned e;  /* table entry flag/number of extra bits */
		unsigned n, d;        /* length and index for copy */
		unsigned w;           /* current window position */
		struct huft *t;       /* pointer to table entry */
		unsigned ml, md;      /* masks for bl and bd bits */
		register unsigned long b;       /* bit buffer */
		register unsigned k;  /* number of bits in bit buffer */

		/* make local copies of globals */
		b = bb;                       /* initialize bit buffer */
		k = bk;
		w = wp;                       /* initialize window position */

		/* inflate the coded data */
		ml = mask_bits[bl];           /* precompute masks for speed */
		md = mask_bits[bd];

		/* do until end of block */
		for (;;){
			NEEDBITS((unsigned)bl)
			if ((e = (t = tl + ((unsigned)b & ml))->e) > 16){
				do {
					if (e == 99)
						return 1;
					DUMPBITS(t->b)
					e -= 16;
					NEEDBITS(e)
				} while ((e = (t = t->v.t + ((unsigned)b & mask_bits[e]))->e) > 16);
			}
			DUMPBITS(t->b)

			/* then it's a literal */
			if (e == 16){
				slide[w++] = (unsigned char)t->v.n;
				if (w == 0x8000){
					printf("flush_output(w);");
					exit(0);
					w = 0;
				}
			}else{
				/* it's an EOB or a length */
				if (e == 15){
					/* exit if end of block */
					break;
				}
				/* get length of block to copy */
				NEEDBITS(e)
				n = t->v.n + ((unsigned)b & mask_bits[e]);
				DUMPBITS(e);

				/* decode distance of block to copy */
				NEEDBITS((unsigned)bd)
				if ((e = (t = td + ((unsigned)b & md))->e) > 16)
					do {
						if (e == 99)
							return 1;
						DUMPBITS(t->b)
						e -= 16;
						NEEDBITS(e)
					} while ((e = (t = t->v.t + ((unsigned)b & mask_bits[e]))->e) > 16);
				DUMPBITS(t->b)
				NEEDBITS(e)
				d = w - t->v.n - ((unsigned)b & mask_bits[e]);
				DUMPBITS(e)

				/* do the copy */
				do {
					n -= (e = (e = 0x8000 - ((d &= 0x8000-1) > w ? d : w)) > n ? n : e);
					do {
						slide[w++] = slide[d++];
					} while (--e);
					if (w == 0x8000){
						printf("flush_output(w);\n");
						exit(0);
						w = 0;
					}
				} while (n);
			}
		}

		/* restore the globals from the locals */
		wp = w;                       /* restore global window pointer */
		bb = b;                       /* restore global bit buffer */
		bk = k;

		/* done */
		return 0;
	}

	int inflate_dynamic(void){
		int i;                /* temporary variables */
		unsigned j;
		unsigned l;           /* last length */
		unsigned m;           /* mask for bit lengths table */
		unsigned n;           /* number of lengths to get */
		PDFUnused unsigned w;           /* current window position */
		struct huft *tl;      /* literal/length code table */
		struct huft *td;      /* distance code table */
		int bl;               /* lookup bits for tl */
		int bd;               /* lookup bits for td */
		unsigned nb;          /* number of bit length codes */
		unsigned nl;          /* number of literal/length codes */
		unsigned nd;          /* number of distance codes */
		unsigned ll[288+32];  /* literal/length and distance code lengths */
		register unsigned long b;       /* bit buffer */
		register unsigned k;  /* number of bits in bit buffer */

		/* make local bit buffer */
		b = bb;
		k = bk;
		w = wp;

		/* read in table lengths */
		NEEDBITS(5)
		nl = 257 + ((unsigned)b & 0x1f);      /* number of literal/length codes */
		DUMPBITS(5)
		NEEDBITS(5)
		nd = 1 + ((unsigned)b & 0x1f);        /* number of distance codes */
		DUMPBITS(5)
		NEEDBITS(4)
		nb = 4 + ((unsigned)b & 0xf);         /* number of bit length codes */
		DUMPBITS(4)
		if (nl > 288 || nd > 32)
			return 1;                   /* bad lengths */

		/* read in bit-length-code lengths */
		for (j = 0; j < nb; j++){
			NEEDBITS(3)
			ll[border[j]] = (unsigned)b & 7;
			DUMPBITS(3)
		}

		for (; j < 19; j++)
			ll[border[j]] = 0;
		
		/* build decoding table for trees--single level, 7 bit lookup */
		bl = 7;
		if ((i = huft_build(ll, 19, 19, NULL, NULL, &tl, &bl)) != 0){
			if (i == 1)
				huft_free(tl);
			return i;                   /* incomplete code set */
		}

		/* Grrrhhh */
		if (tl == NULL)
			return 2;

		/* read in literal and distance code lengths */
		n = nl + nd;
		m = mask_bits[bl];
		i = 0;
		l = 0;
		while ((unsigned)i < n){
			NEEDBITS((unsigned)bl)
			j = (td = tl + ((unsigned)b & m))->b;
			DUMPBITS(j)
			j = td->v.n;
			
			/* length of code in bits (0..15) */
			if (j < 16)
				ll[i++] = l = j;
			else if (j == 16){
				/* repeat last length 3 to 6 times */
				NEEDBITS(2)
				j = 3 + ((unsigned)b & 3);
				DUMPBITS(2)
				if ((unsigned)i + j > n)
					return 1;
				while (j--)
					ll[i++] = l;
			}else if (j == 17){
				/* 3 to 10 zero length codes */
				NEEDBITS(3)
				j = 3 + ((unsigned)b & 7);
				DUMPBITS(3)
				if ((unsigned)i + j > n)
					return 1;
				while (j--)
					ll[i++] = 0;
				l = 0;
			}else{
				/* j == 18: 11 to 138 zero length codes */
				NEEDBITS(7)
				j = 11 + ((unsigned)b & 0x7f);
				DUMPBITS(7)
				if ((unsigned)i + j > n)
					return 1;
				while (j--)
					ll[i++] = 0;
				l = 0;
			}
		}

		/* free decoding table for trees */
		huft_free(tl);

		/* restore the global bit buffer */
		bb = b;
		bk = k;

		/* build the decoding tables for literal/length and distance codes */
		bl = lbits;
		if ((i = huft_build(ll, nl, 257, cplens, cplext, &tl, &bl)) != 0){
			if (i == 1) {
				huft_free(tl);
			}
			return i;                   /* incomplete code set */
		}

		bd = dbits;
		if ((i = huft_build(ll + nl, nd, 0, cpdist, cpdext, &td, &bd)) != 0){
			if (i == 1) {
				i = 0;
			}
			huft_free(tl);
			return i;                   /* incomplete code set */
		}

		/* decompress until an end-of-block code */
		int err = inflate_codes(tl, td, bl, bd) ? 1 : 0;

		/* free the decoding tables */
		huft_free(tl);
		huft_free(td);

		return err;
	}

	int inflate_block(int *e){
		unsigned t;           /* block type */
		PDFUnused unsigned w;           /* current window position */
		register unsigned long b;       /* bit buffer */
		register unsigned k;  /* number of bits in bit buffer */

		/* make local bit buffer */
		b = bb;
		k = bk;
		w = wp;

		/* read in last block bit */
		NEEDBITS(1)
		*e = (int)b & 1;
		DUMPBITS(1)

		/* read in block type */
		NEEDBITS(2)
		t = (unsigned)b & 3;
		DUMPBITS(2)

		/* restore the global bit buffer */
		bb = b;
		bk = k;

		/* inflate that block type */
		if (t == 2){
			return inflate_dynamic();
		}
		if (t == 0){
			printf("return inflate_stored();\n");
			exit(0);
		}
		if (t == 1){
			printf("return inflate_fixed();\n");
			exit(0);
		}

		/* bad block type */
		return 2;
	}

	
	char *pdf_inflate(pdf_stream *file, int length){
		int e;                /* last block flag */
		int r;                /* result code */
		unsigned h;           /* maximum struct huft's malloc'ed */

		in=file;
		insize=length;
		inptr=0;

		//Dropping version bytes
		pdf_read_byte(file);
		pdf_read_byte(file);

		/* initialize window, bit buffer */
		wp = 0;
		bk = 0;
		bb = 0;

		/* decompress until the last block */
		h = 0;
		do {
			hufts = 0;
			if ((r = inflate_block(&e)) != 0){
				printf("return r=%d\n", r);
				exit(0);
			}
			if (hufts > h)
				h = hufts;
		} while (!e);

		while (bk >= 8) {
			bk -= 8;
			inptr--;
		}

		char *s=(char *)slide;
		return s;
	}

	C_MODE_END
