PHP_FUNCTION(stream_is_local)
{
	zval **zstream;
	php_stream *stream = NULL;
	php_stream_wrapper *wrapper = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "Z", &zstream) == FAILURE) {
		RETURN_FALSE;
	}

	if (Z_TYPE_PP(zstream) == IS_RESOURCE) {
		php_stream_from_zval(stream, zstream);
		if (stream == NULL) {
			RETURN_FALSE;
		}
		wrapper = stream->wrapper;
	} else {
		convert_to_string_ex(zstream);

		wrapper = php_stream_locate_url_wrapper(Z_STRVAL_PP(zstream), NULL, 0 TSRMLS_CC);
	}

	if (!wrapper) {
		RETURN_FALSE;
	}

	RETURN_BOOL(wrapper->is_url==0);
}
