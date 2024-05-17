PHP_FUNCTION(strrchr)
{
	zval *needle;
	zend_string *haystack;
	const char *found = NULL;
	zend_long found_offset;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "Sz", &haystack, &needle) == FAILURE) {
		return;
	}

	if (Z_TYPE_P(needle) == IS_STRING) {
		found = zend_memrchr(ZSTR_VAL(haystack), *Z_STRVAL_P(needle), ZSTR_LEN(haystack));
	} else {
		char needle_chr;
		if (php_needle_char(needle, &needle_chr) != SUCCESS) {
			RETURN_FALSE;
		}

		found = zend_memrchr(ZSTR_VAL(haystack),  needle_chr, ZSTR_LEN(haystack));
	}

	if (found) {
		found_offset = found - ZSTR_VAL(haystack);
		RETURN_STRINGL(found, ZSTR_LEN(haystack) - found_offset);
	} else {
		RETURN_FALSE;
	}
}
