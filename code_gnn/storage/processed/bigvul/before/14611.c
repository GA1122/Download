PHP_FUNCTION(explode)
{
	zend_string *str, *delim;
	zend_long limit = ZEND_LONG_MAX;  
	zval tmp;

#ifndef FAST_ZPP
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "SS|l", &delim, &str, &limit) == FAILURE) {
		return;
	}
#else
	ZEND_PARSE_PARAMETERS_START(2, 3)
		Z_PARAM_STR(delim)
		Z_PARAM_STR(str)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(limit)
	ZEND_PARSE_PARAMETERS_END();
#endif

	if (ZSTR_LEN(delim) == 0) {
		php_error_docref(NULL, E_WARNING, "Empty delimiter");
		RETURN_FALSE;
	}

	array_init(return_value);

	if (ZSTR_LEN(str) == 0) {
	  	if (limit >= 0) {
			ZVAL_EMPTY_STRING(&tmp);
			zend_hash_index_add_new(Z_ARRVAL_P(return_value), 0, &tmp);
		}
		return;
	}

	if (limit > 1) {
		php_explode(delim, str, return_value, limit);
	} else if (limit < 0) {
		php_explode_negative_limit(delim, str, return_value, limit);
	} else {
		ZVAL_STR_COPY(&tmp, str);
		zend_hash_index_add_new(Z_ARRVAL_P(return_value), 0, &tmp);
	}
}
