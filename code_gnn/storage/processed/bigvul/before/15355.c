static int php_zlib_decode(const char *in_buf, size_t in_len, char **out_buf, size_t *out_len, int encoding, size_t max_len TSRMLS_DC)
{
	int status = Z_DATA_ERROR;
	z_stream Z;

	memset(&Z, 0, sizeof(z_stream));
	Z.zalloc = php_zlib_alloc;
	Z.zfree = php_zlib_free;

	if (in_len) {
retry_raw_inflate:
		status = inflateInit2(&Z, encoding);
		if (Z_OK == status) {
			Z.next_in = (Bytef *) in_buf;
			Z.avail_in = in_len + 1;  

			switch (status = php_zlib_inflate_rounds(&Z, max_len, out_buf, out_len)) {
				case Z_STREAM_END:
					inflateEnd(&Z);
					return SUCCESS;

				case Z_DATA_ERROR:
					 
					if (PHP_ZLIB_ENCODING_ANY == encoding) {
						inflateEnd(&Z);
						encoding = PHP_ZLIB_ENCODING_RAW;
						goto retry_raw_inflate;
					}
			}
			inflateEnd(&Z);
		}
	}

	*out_buf = NULL;
	*out_len = 0;

	php_error_docref(NULL TSRMLS_CC, E_WARNING, "%s", zError(status));
	return FAILURE;
}
