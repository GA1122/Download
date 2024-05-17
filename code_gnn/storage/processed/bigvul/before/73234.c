int bmp_validate(jas_stream_t *in)
{
	int n;
	int i;
	uchar buf[2];

	assert(JAS_STREAM_MAXPUTBACK >= 2);

	 
	if ((n = jas_stream_read(in, (char *) buf, 2)) < 0) {
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
	 
	if (buf[0] == (BMP_MAGIC & 0xff) && buf[1] == (BMP_MAGIC >> 8)) {
		return 0;
	}
	return -1;
}
