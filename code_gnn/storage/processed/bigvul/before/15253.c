PHP_FUNCTION(stream_socket_accept)
{
	double timeout = FG(default_socket_timeout);
	zval *zpeername = NULL;
	char *peername = NULL;
	int peername_len;
	php_timeout_ull conv;
	struct timeval tv;
	php_stream *stream = NULL, *clistream = NULL;
	zval *zstream;

	char *errstr = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r|dz", &zstream, &timeout, &zpeername) == FAILURE) {
		RETURN_FALSE;
	}

	php_stream_from_zval(stream, &zstream);

	 
	conv = (php_timeout_ull) (timeout * 1000000.0);
#ifdef PHP_WIN32
	tv.tv_sec = (long)(conv / 1000000);
	tv.tv_usec = (long)(conv % 1000000);
#else
	tv.tv_sec = conv / 1000000;
	tv.tv_usec = conv % 1000000;
#endif
	if (zpeername) {
		zval_dtor(zpeername);
		ZVAL_NULL(zpeername);
	}

	if (0 == php_stream_xport_accept(stream, &clistream,
				zpeername ? &peername : NULL,
				zpeername ? &peername_len : NULL,
				NULL, NULL,
				&tv, &errstr
				TSRMLS_CC) && clistream) {

		if (peername) {
			ZVAL_STRINGL(zpeername, peername, peername_len, 0);
		}
		php_stream_to_zval(clistream, return_value);
	} else {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "accept failed: %s", errstr ? errstr : "Unknown error");
		RETVAL_FALSE;
	}

	if (errstr) {
		efree(errstr);
	}
}
