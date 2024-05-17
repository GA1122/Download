PHP_FUNCTION(stream_context_get_params)
{
	zval *zcontext, *options;
	php_stream_context *context;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zcontext) == FAILURE) {
		RETURN_FALSE;
	}

	context = decode_context_param(zcontext TSRMLS_CC);
	if (!context) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid stream/context parameter");
		RETURN_FALSE;
	}

	array_init(return_value);
	if (context->notifier && context->notifier->ptr && context->notifier->func == user_space_stream_notifier) {
		add_assoc_zval_ex(return_value, ZEND_STRS("notification"), context->notifier->ptr);
		Z_ADDREF_P(context->notifier->ptr);
	}
	ALLOC_INIT_ZVAL(options);
	ZVAL_ZVAL(options, context->options, 1, 0);
	add_assoc_zval_ex(return_value, ZEND_STRS("options"), options);
}
