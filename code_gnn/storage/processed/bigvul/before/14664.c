PHP_FUNCTION(substr_compare)
{
	zend_string *s1, *s2;
	zend_long offset, len=0;
	zend_bool cs=0;
	size_t cmp_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "SSl|lb", &s1, &s2, &offset, &len, &cs) == FAILURE) {
		RETURN_FALSE;
	}

	if (ZEND_NUM_ARGS() >= 4 && len <= 0) {
		if (len == 0) {
			RETURN_LONG(0L);
		} else {
			php_error_docref(NULL, E_WARNING, "The length must be greater than or equal to zero");
			RETURN_FALSE;
		}
	}

	if (offset < 0) {
		offset = ZSTR_LEN(s1) + offset;
		offset = (offset < 0) ? 0 : offset;
	}

	if ((size_t)offset >= ZSTR_LEN(s1)) {
		php_error_docref(NULL, E_WARNING, "The start position cannot exceed initial string length");
		RETURN_FALSE;
	}

	cmp_len = (size_t) (len ? len : MAX(ZSTR_LEN(s2), (ZSTR_LEN(s1) - offset)));

	if (!cs) {
		RETURN_LONG(zend_binary_strncmp(ZSTR_VAL(s1) + offset, (ZSTR_LEN(s1) - offset), ZSTR_VAL(s2), ZSTR_LEN(s2), cmp_len));
	} else {
		RETURN_LONG(zend_binary_strncasecmp_l(ZSTR_VAL(s1) + offset, (ZSTR_LEN(s1) - offset), ZSTR_VAL(s2), ZSTR_LEN(s2), cmp_len));
	}
}
