PHP_FUNCTION(strpbrk)
{
	zend_string *haystack, *char_list;
	char *haystack_ptr, *cl_ptr;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "SS", &haystack, &char_list) == FAILURE) {
		RETURN_FALSE;
	}

	if (!ZSTR_LEN(char_list)) {
		php_error_docref(NULL, E_WARNING, "The character list cannot be empty");
		RETURN_FALSE;
	}

	for (haystack_ptr = ZSTR_VAL(haystack); haystack_ptr < (ZSTR_VAL(haystack) + ZSTR_LEN(haystack)); ++haystack_ptr) {
		for (cl_ptr = ZSTR_VAL(char_list); cl_ptr < (ZSTR_VAL(char_list) + ZSTR_LEN(char_list)); ++cl_ptr) {
			if (*cl_ptr == *haystack_ptr) {
				RETURN_STRINGL(haystack_ptr, (ZSTR_VAL(haystack) + ZSTR_LEN(haystack) - haystack_ptr));
			}
		}
	}

	RETURN_FALSE;
}
