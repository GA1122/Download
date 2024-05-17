PHP_FUNCTION(str_repeat)
{
	zend_string		*input_str;		 
	zend_long 		mult;			 
	zend_string	*result;		 
	size_t		result_len;		 

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "Sl", &input_str, &mult) == FAILURE) {
		return;
	}

	if (mult < 0) {
		php_error_docref(NULL, E_WARNING, "Second argument has to be greater than or equal to 0");
		return;
	}

	 
	 
	if (ZSTR_LEN(input_str) == 0 || mult == 0)
		RETURN_EMPTY_STRING();

	 
	result = zend_string_safe_alloc(ZSTR_LEN(input_str), mult, 0, 0);
	result_len = ZSTR_LEN(input_str) * mult;

	 
	if (ZSTR_LEN(input_str) == 1) {
		memset(ZSTR_VAL(result), *ZSTR_VAL(input_str), mult);
	} else {
		char *s, *e, *ee;
		ptrdiff_t l=0;
		memcpy(ZSTR_VAL(result), ZSTR_VAL(input_str), ZSTR_LEN(input_str));
		s = ZSTR_VAL(result);
		e = ZSTR_VAL(result) + ZSTR_LEN(input_str);
		ee = ZSTR_VAL(result) + result_len;

		while (e<ee) {
			l = (e-s) < (ee-e) ? (e-s) : (ee-e);
			memmove(e, s, l);
			e += l;
		}
	}

	ZSTR_VAL(result)[result_len] = '\0';

	RETURN_NEW_STR(result);
}
