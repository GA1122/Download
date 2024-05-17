  int pnm_validate(jas_stream_t *in)
  {
	uchar buf[2];
// 	jas_uchar buf[2];
  	int i;
  	int n;
  
 	assert(JAS_STREAM_MAXPUTBACK >= 2);
 
 	 
 	if ((n = jas_stream_read(in, buf, 2)) < 0) {
 		return -1;
 	}
 	 
 	for (i = n - 1; i >= 0; --i) {
 		if (jas_stream_ungetc(in, buf[i]) == EOF) {
 			return -1;
 		}
 	}
 	 
 	if (n < 2) {
 		return -1;
 	}
 	 
 	if (buf[0] == 'P' && isdigit(buf[1])) {
 		return 0;
 	}
 	return -1;
 }