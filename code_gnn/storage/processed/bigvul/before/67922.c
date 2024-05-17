jas_stream_t *jas_stream_memopen(char *buf, int bufsize)
{
	char *new_buf;
	size_t new_bufsize;

	JAS_DBGLOG(100, ("jas_stream_memopen(%p, %d)\n", buf, bufsize));
	if (bufsize < 0) {
		jas_deprecated("negative buffer size for jas_stream_memopen");
	}
	if (buf && bufsize <= 0) {
		jas_eprintf("Invalid use of jas_stream_memopen detected.\n");
		jas_deprecated("A user-provided buffer for "
		  "jas_stream_memopen cannot be growable.\n");
	}
	if (bufsize <= 0) {
		new_bufsize = 0;
		new_buf = 0;
	} else {
		new_bufsize = bufsize;
		new_buf = buf;
	}
	return jas_stream_memopen2(new_buf, new_bufsize);
}