PHP_FUNCTION(stream_set_timeout)
{
	zval *socket;
	long seconds, microseconds = 0;
	struct timeval t;
	php_stream *stream;
	int argc = ZEND_NUM_ARGS();

	if (zend_parse_parameters(argc TSRMLS_CC, "rl|l", &socket, &seconds, &microseconds) == FAILURE) {
		return;
	}

	php_stream_from_zval(stream, &socket);

	t.tv_sec = seconds;

	if (argc == 3) {
		t.tv_usec = microseconds % 1000000;
		t.tv_sec += microseconds / 1000000;
	} else {
		t.tv_usec = 0;
	}

	if (PHP_STREAM_OPTION_RETURN_OK == php_stream_set_option(stream, PHP_STREAM_OPTION_READ_TIMEOUT, 0, &t)) {
		RETURN_TRUE;
	}

	RETURN_FALSE;
}
