int jas_stream_copy(jas_stream_t *out, jas_stream_t *in, int n)
{
	int all;
	int c;
	int m;

	all = (n < 0) ? 1 : 0;

	m = n;
	while (all || m > 0) {
		if ((c = jas_stream_getc_macro(in)) == EOF) {
			 
			 
			return (!all || jas_stream_error(in)) ? (-1) : 0;
		}
		if (jas_stream_putc_macro(out, c) == EOF) {
			return -1;
		}
		--m;
	}
	return 0;
}
