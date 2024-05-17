static int php_zlib_encode(const char *in_buf, size_t in_len, char **out_buf, size_t *out_len, int encoding, int level TSRMLS_DC)
{
	int status;
	z_stream Z;

	memset(&Z, 0, sizeof(z_stream));
	Z.zalloc = php_zlib_alloc;
	Z.zfree = php_zlib_free;

	if (Z_OK == (status = deflateInit2(&Z, level, Z_DEFLATED, encoding, MAX_MEM_LEVEL, Z_DEFAULT_STRATEGY))) {
		*out_len = PHP_ZLIB_BUFFER_SIZE_GUESS(in_len);
		*out_buf = emalloc(*out_len);

		Z.next_in = (Bytef *) in_buf;
		Z.next_out = (Bytef *) *out_buf;
		Z.avail_in = in_len;
		Z.avail_out = *out_len;

		status = deflate(&Z, Z_FINISH);
		deflateEnd(&Z);

		if (Z_STREAM_END == status) {
			 
			*out_buf = erealloc(*out_buf, Z.total_out + 1);
			(*out_buf)[*out_len = Z.total_out] = '\0';
			return SUCCESS;
		} else {
			efree(*out_buf);
		}
	}

	*out_buf = NULL;
	*out_len = 0;

	php_error_docref(NULL TSRMLS_CC, E_WARNING, "%s", zError(status));
	return FAILURE;
}
