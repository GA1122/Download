PHP_FUNCTION(stream_context_create)
{
	zval *options = NULL, *params = NULL;
	php_stream_context *context;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|a!a!", &options, &params) == FAILURE) {
		RETURN_FALSE;
	}

	context = php_stream_context_alloc(TSRMLS_C);

	if (options) {
		parse_context_options(context, options TSRMLS_CC);
	}

	if (params) {
		parse_context_params(context, params TSRMLS_CC);
	}

	RETURN_RESOURCE(context->rsrc_id);
}
