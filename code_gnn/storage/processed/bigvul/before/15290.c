static void user_space_stream_notifier(php_stream_context *context, int notifycode, int severity,
		char *xmsg, int xcode, size_t bytes_sofar, size_t bytes_max, void * ptr TSRMLS_DC)
{
	zval *callback = (zval*)context->notifier->ptr;
	zval *retval = NULL;
	zval zvs[6];
	zval *ps[6];
	zval **ptps[6];
	int i;

	for (i = 0; i < 6; i++) {
		INIT_ZVAL(zvs[i]);
		ps[i] = &zvs[i];
		ptps[i] = &ps[i];
		MAKE_STD_ZVAL(ps[i]);
	}

	ZVAL_LONG(ps[0], notifycode);
	ZVAL_LONG(ps[1], severity);
	if (xmsg) {
		ZVAL_STRING(ps[2], xmsg, 1);
	} else {
		ZVAL_NULL(ps[2]);
	}
	ZVAL_LONG(ps[3], xcode);
	ZVAL_LONG(ps[4], bytes_sofar);
	ZVAL_LONG(ps[5], bytes_max);

	if (FAILURE == call_user_function_ex(EG(function_table), NULL, callback, &retval, 6, ptps, 0, NULL TSRMLS_CC)) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "failed to call user notifier");
	}
	for (i = 0; i < 6; i++) {
		zval_ptr_dtor(&ps[i]);
	}
	if (retval) {
		zval_ptr_dtor(&retval);
	}
}
