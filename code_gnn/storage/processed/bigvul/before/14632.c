PHP_FUNCTION(ucfirst)
{
	zend_string *str;

#ifndef FAST_ZPP
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "S", &str) == FAILURE) {
		return;
	}
#else
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(str)
	ZEND_PARSE_PARAMETERS_END();
#endif

	if (!ZSTR_LEN(str)) {
		RETURN_EMPTY_STRING();
	}

	ZVAL_STRINGL(return_value, ZSTR_VAL(str), ZSTR_LEN(str));
	php_ucfirst(Z_STRVAL_P(return_value));
}
