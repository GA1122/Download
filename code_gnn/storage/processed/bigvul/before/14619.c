PHP_FUNCTION(stristr)
{
	zval *needle;
	zend_string *haystack;
	char *found = NULL;
	size_t  found_offset;
	char *haystack_dup;
	char needle_char[2];
	zend_bool part = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "Sz|b", &haystack, &needle, &part) == FAILURE) {
		return;
	}

	haystack_dup = estrndup(ZSTR_VAL(haystack), ZSTR_LEN(haystack));

	if (Z_TYPE_P(needle) == IS_STRING) {
		char *orig_needle;
		if (!Z_STRLEN_P(needle)) {
			php_error_docref(NULL, E_WARNING, "Empty needle");
			efree(haystack_dup);
			RETURN_FALSE;
		}
		orig_needle = estrndup(Z_STRVAL_P(needle), Z_STRLEN_P(needle));
		found = php_stristr(haystack_dup, orig_needle, ZSTR_LEN(haystack), Z_STRLEN_P(needle));
		efree(orig_needle);
	} else {
		if (php_needle_char(needle, needle_char) != SUCCESS) {
			efree(haystack_dup);
			RETURN_FALSE;
		}
		needle_char[1] = 0;

		found = php_stristr(haystack_dup, needle_char, ZSTR_LEN(haystack), 1);
	}

	if (found) {
		found_offset = found - haystack_dup;
		if (part) {
			RETVAL_STRINGL(ZSTR_VAL(haystack), found_offset);
		} else {
			RETVAL_STRINGL(ZSTR_VAL(haystack) + found_offset, ZSTR_LEN(haystack) - found_offset);
		}
	} else {
		RETVAL_FALSE;
	}

	efree(haystack_dup);
}
