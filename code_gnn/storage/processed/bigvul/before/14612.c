PHP_FUNCTION(implode)
{
	zval *arg1, *arg2 = NULL, *arr;
	zend_string *delim;

#ifndef FAST_ZPP
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "z|z", &arg1, &arg2) == FAILURE) {
		return;
	}
#else
	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_ZVAL(arg1)
		Z_PARAM_OPTIONAL
		Z_PARAM_ZVAL(arg2)
	ZEND_PARSE_PARAMETERS_END();
#endif

	if (arg2 == NULL) {
		if (Z_TYPE_P(arg1) != IS_ARRAY) {
			php_error_docref(NULL, E_WARNING, "Argument must be an array");
			return;
		}

		delim = ZSTR_EMPTY_ALLOC();
		arr = arg1;
	} else {
		if (Z_TYPE_P(arg1) == IS_ARRAY) {
			delim = zval_get_string(arg2);
			arr = arg1;
		} else if (Z_TYPE_P(arg2) == IS_ARRAY) {
			delim = zval_get_string(arg1);
			arr = arg2;
		} else {
			php_error_docref(NULL, E_WARNING, "Invalid arguments passed");
			return;
		}
	}

	php_implode(delim, arr, return_value);
	zend_string_release(delim);
}
