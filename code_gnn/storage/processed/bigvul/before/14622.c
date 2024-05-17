PHP_FUNCTION(stripos)
{
	char *found = NULL;
	zend_string *haystack;
	zend_long offset = 0;
	char needle_char[2];
	zval *needle;
	zend_string *needle_dup = NULL, *haystack_dup;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "Sz|l", &haystack, &needle, &offset) == FAILURE) {
		return;
	}

	if (offset < 0 || (size_t)offset > ZSTR_LEN(haystack)) {
		php_error_docref(NULL, E_WARNING, "Offset not contained in string");
		RETURN_FALSE;
	}

	if (ZSTR_LEN(haystack) == 0) {
		RETURN_FALSE;
	}

	if (Z_TYPE_P(needle) == IS_STRING) {
		if (Z_STRLEN_P(needle) == 0 || Z_STRLEN_P(needle) > ZSTR_LEN(haystack)) {
			RETURN_FALSE;
		}

		haystack_dup = php_string_tolower(haystack);
		needle_dup = php_string_tolower(Z_STR_P(needle));
		found = (char*)php_memnstr(ZSTR_VAL(haystack_dup) + offset,
				ZSTR_VAL(needle_dup), ZSTR_LEN(needle_dup), ZSTR_VAL(haystack_dup) + ZSTR_LEN(haystack));
	} else {
		if (php_needle_char(needle, needle_char) != SUCCESS) {
			RETURN_FALSE;
		}
		haystack_dup = php_string_tolower(haystack);
		needle_char[0] = tolower(needle_char[0]);
		needle_char[1] = '\0';
		found = (char*)php_memnstr(ZSTR_VAL(haystack_dup) + offset,
							needle_char,
							sizeof(needle_char) - 1,
							ZSTR_VAL(haystack_dup) + ZSTR_LEN(haystack));
	}


	if (found) {
		RETVAL_LONG(found - ZSTR_VAL(haystack_dup));
	} else {
		RETVAL_FALSE;
	}

	zend_string_release(haystack_dup);
	if (needle_dup) {
		zend_string_release(needle_dup);
	}
}
