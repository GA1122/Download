static int parse_context_params(php_stream_context *context, zval *params TSRMLS_DC)
{
	int ret = SUCCESS;
	zval **tmp;

	if (SUCCESS == zend_hash_find(Z_ARRVAL_P(params), "notification", sizeof("notification"), (void**)&tmp)) {

		if (context->notifier) {
			php_stream_notification_free(context->notifier);
			context->notifier = NULL;
		}

		context->notifier = php_stream_notification_alloc();
		context->notifier->func = user_space_stream_notifier;
		context->notifier->ptr = *tmp;
		Z_ADDREF_P(*tmp);
		context->notifier->dtor = user_space_stream_notifier_dtor;
	}
	if (SUCCESS == zend_hash_find(Z_ARRVAL_P(params), "options", sizeof("options"), (void**)&tmp)) {
		if (Z_TYPE_PP(tmp) == IS_ARRAY) {
			parse_context_options(context, *tmp TSRMLS_CC);
		} else {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid stream/context parameter");
		}
	}

	return ret;
}
