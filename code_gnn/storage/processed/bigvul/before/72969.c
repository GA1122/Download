static int pnm_getuintstr(jas_stream_t *in, uint_fast32_t *val)
{
	uint_fast32_t v;
	int c;

	 
	do {
		if ((c = pnm_getc(in)) == EOF) {
			return -1;
		}
	} while (isspace(c));

	 
	v = 0;
	while (isdigit(c)) {
		v = 10 * v + c - '0';
		if ((c = pnm_getc(in)) < 0) {
			return -1;
		}
	}

	 
	if (!isspace(c)) {
		return -1;
	}

	if (val) {
		*val = v;
	}
	return 0;
}
