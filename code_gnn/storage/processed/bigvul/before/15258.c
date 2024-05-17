PHP_FUNCTION(stream_copy_to_stream)
{
	php_stream *src, *dest;
	zval *zsrc, *zdest;
	long maxlen = PHP_STREAM_COPY_ALL, pos = 0;
	size_t len;
	int ret;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rr|ll", &zsrc, &zdest, &maxlen, &pos) == FAILURE) {
		RETURN_FALSE;
	}

	php_stream_from_zval(src, &zsrc);
	php_stream_from_zval(dest, &zdest);

	if (pos > 0 && php_stream_seek(src, pos, SEEK_SET) < 0) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Failed to seek to position %ld in the stream", pos);
		RETURN_FALSE;
	}

	ret = php_stream_copy_to_stream_ex(src, dest, maxlen, &len);

	if (ret != SUCCESS) {
		RETURN_FALSE;
	}
	RETURN_LONG(len);
}
