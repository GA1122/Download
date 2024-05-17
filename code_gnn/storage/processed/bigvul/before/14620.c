PHP_FUNCTION(strstr)
{
	zval *needle;
	zend_string *haystack;
	char *found = NULL;
	char needle_char[2];
	zend_long found_offset;
	zend_bool part = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "Sz|b", &haystack, &needle, &part) == FAILURE) {
		return;
	}

	if (Z_TYPE_P(needle) == IS_STRING) {
		if (!Z_STRLEN_P(needle)) {
			php_error_docref(NULL, E_WARNING, "Empty needle");
			RETURN_FALSE;
		}

		found = (char*)php_memnstr(ZSTR_VAL(haystack), Z_STRVAL_P(needle), Z_STRLEN_P(needle), ZSTR_VAL(haystack) + ZSTR_LEN(haystack));
	} else {
		if (php_needle_char(needle, needle_char) != SUCCESS) {
			RETURN_FALSE;
		}
		needle_char[1] = 0;

		found = (char*)php_memnstr(ZSTR_VAL(haystack), needle_char, 1, ZSTR_VAL(haystack) + ZSTR_LEN(haystack));
	}

	if (found) {
		found_offset = found - ZSTR_VAL(haystack);
		if (part) {
			RETURN_STRINGL(ZSTR_VAL(haystack), found_offset);
		} else {
			RETURN_STRINGL(found, ZSTR_LEN(haystack) - found_offset);
		}
	}
	RETURN_FALSE;
}
