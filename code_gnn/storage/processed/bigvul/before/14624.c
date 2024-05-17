PHP_FUNCTION(strripos)
{
	zval *zneedle;
	zend_string *needle;
	zend_string *haystack;
	zend_long offset = 0;
	char *p, *e;
	char *found;
	zend_string *needle_dup, *haystack_dup, *ord_needle = NULL;
	ALLOCA_FLAG(use_heap);


	if (zend_parse_parameters(ZEND_NUM_ARGS(), "Sz|l", &haystack, &zneedle, &offset) == FAILURE) {
		RETURN_FALSE;
	}

	ZSTR_ALLOCA_ALLOC(ord_needle, 1, use_heap);
	if (Z_TYPE_P(zneedle) == IS_STRING) {
		needle = Z_STR_P(zneedle);
	} else {
		if (php_needle_char(zneedle, ZSTR_VAL(ord_needle)) != SUCCESS) {
			ZSTR_ALLOCA_FREE(ord_needle, use_heap);
			RETURN_FALSE;
		}
		ZSTR_VAL(ord_needle)[1] = '\0';
		needle = ord_needle;
	}

	if ((ZSTR_LEN(haystack) == 0) || (ZSTR_LEN(needle) == 0)) {
		ZSTR_ALLOCA_FREE(ord_needle, use_heap);
		RETURN_FALSE;
	}

	if (ZSTR_LEN(needle) == 1) {
		 
		if (offset >= 0) {
			if ((size_t)offset > ZSTR_LEN(haystack)) {
				ZSTR_ALLOCA_FREE(ord_needle, use_heap);
				php_error_docref(NULL, E_WARNING, "Offset is greater than the length of haystack string");
				RETURN_FALSE;
			}
			p = ZSTR_VAL(haystack) + (size_t)offset;
			e = ZSTR_VAL(haystack) + ZSTR_LEN(haystack) - 1;
		} else {
			p = ZSTR_VAL(haystack);
			if (offset < -INT_MAX || (size_t)(-offset) > ZSTR_LEN(haystack)) {
				ZSTR_ALLOCA_FREE(ord_needle, use_heap);
				php_error_docref(NULL, E_WARNING, "Offset is greater than the length of haystack string");
				RETURN_FALSE;
			}
			e = ZSTR_VAL(haystack) + ZSTR_LEN(haystack) + (size_t)offset;
		}
		 
		*ZSTR_VAL(ord_needle) = tolower(*ZSTR_VAL(needle));
		while (e >= p) {
			if (tolower(*e) == *ZSTR_VAL(ord_needle)) {
				ZSTR_ALLOCA_FREE(ord_needle, use_heap);
				RETURN_LONG(e - p + (offset > 0 ? offset : 0));
			}
			e--;
		}
		ZSTR_ALLOCA_FREE(ord_needle, use_heap);
		RETURN_FALSE;
	}

	haystack_dup = php_string_tolower(haystack);
	if (offset >= 0) {
		if ((size_t)offset > ZSTR_LEN(haystack)) {
			zend_string_release(haystack_dup);
			ZSTR_ALLOCA_FREE(ord_needle, use_heap);
			php_error_docref(NULL, E_WARNING, "Offset is greater than the length of haystack string");
			RETURN_FALSE;
		}
		p = ZSTR_VAL(haystack_dup) + offset;
		e = ZSTR_VAL(haystack_dup) + ZSTR_LEN(haystack);
	} else {
		if (offset < -INT_MAX || (size_t)(-offset) > ZSTR_LEN(haystack)) {
			zend_string_release(haystack_dup);
			ZSTR_ALLOCA_FREE(ord_needle, use_heap);
			php_error_docref(NULL, E_WARNING, "Offset is greater than the length of haystack string");
			RETURN_FALSE;
		}
		p = ZSTR_VAL(haystack_dup);
		if (-offset < ZSTR_LEN(needle)) {
			e = ZSTR_VAL(haystack_dup) + ZSTR_LEN(haystack);
		} else {
			e = ZSTR_VAL(haystack_dup) + ZSTR_LEN(haystack) + offset + ZSTR_LEN(needle);
		}
	}

	needle_dup = php_string_tolower(needle);
	if ((found = (char *)zend_memnrstr(p, ZSTR_VAL(needle_dup), ZSTR_LEN(needle_dup), e))) {
		RETVAL_LONG(found - ZSTR_VAL(haystack_dup));
		zend_string_release(needle_dup);
		zend_string_release(haystack_dup);
		ZSTR_ALLOCA_FREE(ord_needle, use_heap);
	} else {
		zend_string_release(needle_dup);
		zend_string_release(haystack_dup);
		ZSTR_ALLOCA_FREE(ord_needle, use_heap);
		RETURN_FALSE;
	}
}
