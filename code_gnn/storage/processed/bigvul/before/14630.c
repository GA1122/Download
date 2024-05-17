PHP_FUNCTION(ord)
{
	char   *str;
	size_t str_len;

#ifndef FAST_ZPP
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &str, &str_len) == FAILURE) {
		return;
	}
#else
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STRING(str, str_len)
	ZEND_PARSE_PARAMETERS_END();
#endif

	RETURN_LONG((unsigned char) str[0]);
}
