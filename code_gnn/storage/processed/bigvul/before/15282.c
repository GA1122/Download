PHP_FUNCTION(stream_socket_shutdown)
{
	long how;
	zval *zstream;
	php_stream *stream;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rl", &zstream, &how) == FAILURE) {
		RETURN_FALSE;
	}

	if (how != STREAM_SHUT_RD &&
	    how != STREAM_SHUT_WR &&
	    how != STREAM_SHUT_RDWR) {
		RETURN_FALSE;
	}

	php_stream_from_zval(stream, &zstream);

	RETURN_BOOL(php_stream_xport_shutdown(stream, (stream_shutdown_t)how TSRMLS_CC) == 0);
}
