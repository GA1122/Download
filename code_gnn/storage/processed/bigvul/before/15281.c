PHP_FUNCTION(stream_supports_lock)
{
	php_stream *stream;
	zval *zsrc;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zsrc) == FAILURE) {
		RETURN_FALSE;
	}

	php_stream_from_zval(stream, &zsrc);

	if (!php_stream_supports_lock(stream)) {
		RETURN_FALSE;
	}

	RETURN_TRUE;
}
