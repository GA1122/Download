  int jas_stream_pad(jas_stream_t *stream, int n, int c)
  {
  	int m;
// 	if (n < 0) {
// 		jas_deprecated("negative count for jas_stream_pad");
// 	}
  	m = n;
  	for (m = n; m > 0; --m) {
  		if (jas_stream_putc(stream, c) == EOF)
 			return n - m;
 	}
 	return n;
 }