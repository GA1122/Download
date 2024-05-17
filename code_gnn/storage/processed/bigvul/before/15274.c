PHP_FUNCTION(stream_set_blocking)
{
	zval *arg1;
	int block;
	long arg2;
	php_stream *stream;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rl", &arg1, &arg2) == FAILURE) {
		return;
	}

	php_stream_from_zval(stream, &arg1);

	block = arg2;

	if (php_stream_set_option(stream, PHP_STREAM_OPTION_BLOCKING, block == 0 ? 0 : 1, NULL) == -1) {
		RETURN_FALSE;
	}

	RETURN_TRUE;
}
