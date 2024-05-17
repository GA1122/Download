PHP_FUNCTION(flock)
{
	zval *arg1, *arg3 = NULL;
	int act;
	php_stream *stream;
	long operation = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rl|z", &arg1, &operation, &arg3) == FAILURE) {
		return;
	}

	PHP_STREAM_TO_ZVAL(stream, &arg1);

	act = operation & 3;
	if (act < 1 || act > 3) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Illegal operation argument");
		RETURN_FALSE;
	}

	if (arg3 && PZVAL_IS_REF(arg3)) {
		convert_to_long_ex(&arg3);
		Z_LVAL_P(arg3) = 0;
	}

	 
	act = flock_values[act - 1] | (operation & PHP_LOCK_NB ? LOCK_NB : 0);
	if (php_stream_lock(stream, act)) {
		if (operation && errno == EWOULDBLOCK && arg3 && PZVAL_IS_REF(arg3)) {
			Z_LVAL_P(arg3) = 1;
		}
		RETURN_FALSE;
	}
	RETURN_TRUE;
}