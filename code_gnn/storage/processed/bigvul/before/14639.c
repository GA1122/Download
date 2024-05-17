PHP_FUNCTION(addslashes)
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

	if (ZSTR_LEN(str) == 0) {
		RETURN_EMPTY_STRING();
	}

	RETURN_STR(php_addslashes(str, 0));
}
