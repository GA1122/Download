PHP_FUNCTION(chr)
{
	zend_long c;

	if (ZEND_NUM_ARGS() != 1) {
		WRONG_PARAM_COUNT;
	}

#ifndef FAST_ZPP
	if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS(), "l", &c) == FAILURE) {
		c = 0;
	}
#else
	ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_QUIET, 1, 1)
		Z_PARAM_LONG(c)
	ZEND_PARSE_PARAMETERS_END_EX(c = 0);
#endif

	c &= 0xff;
	if (CG(one_char_string)[c]) {
		ZVAL_INTERNED_STR(return_value, CG(one_char_string)[c]);
	} else {
		ZVAL_NEW_STR(return_value, zend_string_alloc(1, 0));
		Z_STRVAL_P(return_value)[0] = (char)c;
		Z_STRVAL_P(return_value)[1] = '\0';
	}
}
