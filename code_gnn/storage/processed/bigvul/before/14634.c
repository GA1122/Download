PHP_FUNCTION(ucwords)
{
	zend_string *str;
	char *delims = " \t\r\n\f\v";
	register char *r, *r_end;
	size_t delims_len = 6;
	char mask[256];

#ifndef FAST_ZPP
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "S|s", &str, &delims, &delims_len) == FAILURE) {
		return;
	}
#else
	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_STR(str)
		Z_PARAM_OPTIONAL
		Z_PARAM_STRING(delims, delims_len)
	ZEND_PARSE_PARAMETERS_END();
#endif

	if (!ZSTR_LEN(str)) {
		RETURN_EMPTY_STRING();
	}

	php_charmask((unsigned char *)delims, delims_len, mask);

	ZVAL_STRINGL(return_value, ZSTR_VAL(str), ZSTR_LEN(str));
	r = Z_STRVAL_P(return_value);

	*r = toupper((unsigned char) *r);
	for (r_end = r + Z_STRLEN_P(return_value) - 1; r < r_end; ) {
		if (mask[(unsigned char)*r++]) {
			*r = toupper((unsigned char) *r);
		}
	}
}
