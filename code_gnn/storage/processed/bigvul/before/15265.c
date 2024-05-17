PHP_FUNCTION(stream_context_set_params)
{
	zval *params, *zcontext;
	php_stream_context *context;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ra", &zcontext, &params) == FAILURE) {
		RETURN_FALSE;
	}

	context = decode_context_param(zcontext TSRMLS_CC);
	if (!context) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid stream/context parameter");
		RETURN_FALSE;
	}

	RETVAL_BOOL(parse_context_params(context, params TSRMLS_CC) == SUCCESS);
}
