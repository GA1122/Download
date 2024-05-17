static PHP_FUNCTION(session_register_shutdown)
{
	php_shutdown_function_entry shutdown_function_entry;
	zval *callback;

	 

	shutdown_function_entry.arg_count = 1;
	shutdown_function_entry.arguments = (zval **) safe_emalloc(sizeof(zval *), 1, 0);

	MAKE_STD_ZVAL(callback);
	ZVAL_STRING(callback, "session_write_close", 1);
	shutdown_function_entry.arguments[0] = callback;

	if (!append_user_shutdown_function(shutdown_function_entry TSRMLS_CC)) {
		zval_ptr_dtor(&callback);
		efree(shutdown_function_entry.arguments);

		 
		php_session_flush(TSRMLS_C);
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unable to register session flush function");
	}
}
