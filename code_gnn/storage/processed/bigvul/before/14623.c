PHP_FUNCTION(strrpos)
{
	zval *zneedle;
	char *needle;
	zend_string *haystack;
	size_t needle_len;
	zend_long offset = 0;
	char *p, *e, ord_needle[2];
	char *found;

#ifndef FAST_ZPP
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "Sz|l", &haystack, &zneedle, &offset) == FAILURE) {
		RETURN_FALSE;
	}
#else
	ZEND_PARSE_PARAMETERS_START(2, 3)
		Z_PARAM_STR(haystack)
		Z_PARAM_ZVAL(zneedle)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(offset)
	ZEND_PARSE_PARAMETERS_END_EX(RETURN_FALSE);
#endif

	if (Z_TYPE_P(zneedle) == IS_STRING) {
		needle = Z_STRVAL_P(zneedle);
		needle_len = Z_STRLEN_P(zneedle);
	} else {
		if (php_needle_char(zneedle, ord_needle) != SUCCESS) {
			RETURN_FALSE;
		}
		ord_needle[1] = '\0';
		needle = ord_needle;
		needle_len = 1;
	}

	if ((ZSTR_LEN(haystack) == 0) || (needle_len == 0)) {
		RETURN_FALSE;
	}

	if (offset >= 0) {
		if ((size_t)offset > ZSTR_LEN(haystack)) {
			php_error_docref(NULL, E_WARNING, "Offset is greater than the length of haystack string");
			RETURN_FALSE;
		}
		p = ZSTR_VAL(haystack) + (size_t)offset;
		e = ZSTR_VAL(haystack) + ZSTR_LEN(haystack);
	} else {
		if (offset < -INT_MAX || (size_t)(-offset) > ZSTR_LEN(haystack)) {
			php_error_docref(NULL, E_WARNING, "Offset is greater than the length of haystack string");
			RETURN_FALSE;
		}
		p = ZSTR_VAL(haystack);
		if (-offset < needle_len) {
			e = ZSTR_VAL(haystack) + ZSTR_LEN(haystack);
		} else {
			e = ZSTR_VAL(haystack) + ZSTR_LEN(haystack) + offset + needle_len;
		}
	}

	if ((found = (char *)zend_memnrstr(p, needle, needle_len, e))) {
		RETURN_LONG(found - ZSTR_VAL(haystack));
	}

	RETURN_FALSE;
}
