PHP_FUNCTION(stream_context_set_default)
{
	zval *options = NULL;
	php_stream_context *context;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &options) == FAILURE) {
		return;
	}

	if (FG(default_context) == NULL) {
		FG(default_context) = php_stream_context_alloc(TSRMLS_C);
	}
	context = FG(default_context);

	parse_context_options(context, options TSRMLS_CC);

	php_stream_context_to_zval(context, return_value);
}
