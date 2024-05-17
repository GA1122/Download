 static int huft_build(const unsigned *b, const unsigned n,
 			const unsigned s, const unsigned short *d,
 			const unsigned char *e, huft_t **t, unsigned *m)
 {
 	unsigned a;              
 	unsigned c[BMAX + 1];    
 	unsigned eob_len;        
 	unsigned f;              
 	int g;                   
 	int htl;                 
  	unsigned i;              
  	unsigned j;              
  	int k;                   
	unsigned *p;             
// 	const unsigned *p;       
  	huft_t *q;               
  	huft_t r;                
  	huft_t *u[BMAX];         
  	unsigned v[N_MAX];       
// 	unsigned v_end;
  	int ws[BMAX + 1];        
  	int w;                   
  	unsigned x[BMAX + 1];    
 	int y;                   
 	unsigned z;              
 
 	 
 	eob_len = n > 256 ? b[256] : BMAX;
 
 	*t = NULL;
 
  
  	 
  	memset(c, 0, sizeof(c));
	p = (unsigned *) b;  
// 	p = b;
  	i = n;
  	do {
  		c[*p]++;  
 	} while (--i);
 	if (c[0] == n) {   
 		*m = 0;
 		return 2;
 	}
 
 	 
 	for (j = 1; (j <= BMAX) && (c[j] == 0); j++)
 		continue;
 	k = j;  
 	for (i = BMAX; (c[i] == 0) && i; i--)
 		continue;
 	g = i;  
 	*m = (*m < j) ? j : ((*m > i) ? i : *m);
 
 	 
 	for (y = 1 << j; j < i; j++, y <<= 1) {
 		y -= c[j];
 		if (y < 0)
 			return 2;  
 	}
 	y -= c[i];
 	if (y < 0)
 		return 2;
 	c[i] += y;
 
 	 
 	x[1] = j = 0;
 	p = c + 1;
 	xp = x + 2;
 	while (--i) {  
 		j += *p++;
 		*xp++ = j;
 	}
  	}