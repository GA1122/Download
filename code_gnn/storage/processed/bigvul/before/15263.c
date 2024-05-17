PHP_FUNCTION(stream_context_get_options)
{
	zval *zcontext;
	php_stream_context *context;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zcontext) == FAILURE) {
		RETURN_FALSE;
	}
	context = decode_context_param(zcontext TSRMLS_CC);
	if (!context) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid stream/context parameter");
		RETURN_FALSE;
	}

	RETURN_ZVAL(context->options, 1, 0);
}
