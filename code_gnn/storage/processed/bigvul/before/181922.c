 static int mem_resize(jas_stream_memobj_t *m, int bufsize)
// static int mem_resize(jas_stream_memobj_t *m, size_t bufsize)
  {
  	unsigned char *buf;
  
	assert(bufsize >= 0);
// 	 
// 
// 	JAS_DBGLOG(100, ("mem_resize(%p, %zu)\n", m, bufsize));
// 	if (!bufsize) {
// 		jas_eprintf(
// 		  "mem_resize was not really designed to handle a buffer of size 0\n"
// 		  "This may not work.\n"
// 		);
// 	}
  
	JAS_DBGLOG(100, ("mem_resize(%p, %d)\n", m, bufsize));
  	if (!(buf = jas_realloc2(m->buf_, bufsize, sizeof(unsigned char))) &&
  	  bufsize) {
  		JAS_DBGLOG(100, ("mem_resize realloc failed\n"));
 		return -1;
 	}
 	JAS_DBGLOG(100, ("mem_resize realloc succeeded\n"));
 	m->buf_ = buf;
 	m->bufsize_ = bufsize;
 	return 0;
 }