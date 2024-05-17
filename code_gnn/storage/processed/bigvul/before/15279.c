PHP_FUNCTION(stream_socket_enable_crypto)
{
	long cryptokind = 0;
	zval *zstream, *zsessstream = NULL;
	php_stream *stream, *sessstream = NULL;
	zend_bool enable;
	int ret;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rb|lr", &zstream, &enable, &cryptokind, &zsessstream) == FAILURE) {
		RETURN_FALSE;
	}

	php_stream_from_zval(stream, &zstream);

	if (ZEND_NUM_ARGS() >= 3) {
		if (zsessstream) {
			php_stream_from_zval(sessstream, &zsessstream);
		}

		if (php_stream_xport_crypto_setup(stream, cryptokind, sessstream TSRMLS_CC) < 0) {
			RETURN_FALSE;
		}
	} else if (enable) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "When enabling encryption you must specify the crypto type");
		RETURN_FALSE;
	}

	ret = php_stream_xport_crypto_enable(stream, enable TSRMLS_CC);
	switch (ret) {
		case -1:
			RETURN_FALSE;

		case 0:
			RETURN_LONG(0);

		default:
			RETURN_TRUE;
	}
}
