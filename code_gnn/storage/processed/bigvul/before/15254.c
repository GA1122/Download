PHP_FUNCTION(stream_socket_get_name)
{
	php_stream *stream;
	zval *zstream;
	zend_bool want_peer;
	char *name = NULL;
	int name_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rb", &zstream, &want_peer) == FAILURE) {
		RETURN_FALSE;
	}

	php_stream_from_zval(stream, &zstream);

	if (0 != php_stream_xport_get_name(stream, want_peer,
				&name,
				&name_len,
				NULL, NULL
				TSRMLS_CC)) {
		RETURN_FALSE;
	}

	RETURN_STRINGL(name, name_len, 0);
}
