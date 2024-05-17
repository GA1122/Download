PHP_FUNCTION(stream_socket_recvfrom)
{
	php_stream *stream;
	zval *zstream, *zremote = NULL;
	char *remote_addr = NULL;
	int remote_addr_len;
	long to_read = 0;
	char *read_buf;
	long flags = 0;
	int recvd;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rl|lz", &zstream, &to_read, &flags, &zremote) == FAILURE) {
		RETURN_FALSE;
	}

	php_stream_from_zval(stream, &zstream);

	if (zremote) {
		zval_dtor(zremote);
		ZVAL_NULL(zremote);
	}

	if (to_read <= 0) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Length parameter must be greater than 0");
		RETURN_FALSE;
	}

	read_buf = safe_emalloc(1, to_read, 1);

	recvd = php_stream_xport_recvfrom(stream, read_buf, to_read, flags, NULL, NULL,
			zremote ? &remote_addr : NULL,
			zremote ? &remote_addr_len : NULL
			TSRMLS_CC);

	if (recvd >= 0) {
		if (zremote) {
			ZVAL_STRINGL(zremote, remote_addr, remote_addr_len, 0);
		}
		read_buf[recvd] = '\0';

		RETURN_STRINGL(read_buf, recvd, 0);
	}

	efree(read_buf);
	RETURN_FALSE;
}
