get_filter(struct archive *_a, int n)
{
	struct archive_read *a = (struct archive_read *)_a;
	struct archive_read_filter *f = a->filter;
	 
	if (n == -1 && f != NULL) {
		struct archive_read_filter *last = f;
		f = f->upstream;
		while (f != NULL) {
			last = f;
			f = f->upstream;
		}
		return (last);
	}
	if (n < 0)
		return NULL;
	while (n > 0 && f != NULL) {
		f = f->upstream;
		--n;
	}
	return (f);
}
