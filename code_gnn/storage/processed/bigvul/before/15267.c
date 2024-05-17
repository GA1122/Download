PHP_FUNCTION(stream_context_get_default)
{
	zval *params = NULL;
	php_stream_context *context;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|a", &params) == FAILURE) {
		RETURN_FALSE;
	}

	if (FG(default_context) == NULL) {
		FG(default_context) = php_stream_context_alloc(TSRMLS_C);
	}
	context = FG(default_context);

	if (params) {
		parse_context_options(context, params TSRMLS_CC);
	}

	php_stream_context_to_zval(context, return_value);
}
