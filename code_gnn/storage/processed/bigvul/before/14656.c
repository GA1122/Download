PHP_FUNCTION(str_pad)
{
	 
	zend_string *input;				 
	zend_long pad_length;			 

	 
	size_t num_pad_chars;		 
	char *pad_str = " ";  
	size_t pad_str_len = 1;
	zend_long   pad_type_val = STR_PAD_RIGHT;  
	size_t	   i, left_pad=0, right_pad=0;
	zend_string *result = NULL;	 

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "Sl|sl", &input, &pad_length, &pad_str, &pad_str_len, &pad_type_val) == FAILURE) {
		return;
	}

	 
	if (pad_length < 0  || (size_t)pad_length <= ZSTR_LEN(input)) {
		RETURN_STRINGL(ZSTR_VAL(input), ZSTR_LEN(input));
	}

	if (pad_str_len == 0) {
		php_error_docref(NULL, E_WARNING, "Padding string cannot be empty");
		return;
	}

	if (pad_type_val < STR_PAD_LEFT || pad_type_val > STR_PAD_BOTH) {
		php_error_docref(NULL, E_WARNING, "Padding type has to be STR_PAD_LEFT, STR_PAD_RIGHT, or STR_PAD_BOTH");
		return;
	}

	num_pad_chars = pad_length - ZSTR_LEN(input);
	if (num_pad_chars >= INT_MAX) {
		php_error_docref(NULL, E_WARNING, "Padding length is too long");
		return;
	}

	result = zend_string_alloc(ZSTR_LEN(input) + num_pad_chars, 0);
	ZSTR_LEN(result) = 0;

	 
	switch (pad_type_val) {
		case STR_PAD_RIGHT:
			left_pad = 0;
			right_pad = num_pad_chars;
			break;

		case STR_PAD_LEFT:
			left_pad = num_pad_chars;
			right_pad = 0;
			break;

		case STR_PAD_BOTH:
			left_pad = num_pad_chars / 2;
			right_pad = num_pad_chars - left_pad;
			break;
	}

	 
	for (i = 0; i < left_pad; i++)
		ZSTR_VAL(result)[ZSTR_LEN(result)++] = pad_str[i % pad_str_len];

	 
	memcpy(ZSTR_VAL(result) + ZSTR_LEN(result), ZSTR_VAL(input), ZSTR_LEN(input));
	ZSTR_LEN(result) += ZSTR_LEN(input);

	 
	for (i = 0; i < right_pad; i++)
		ZSTR_VAL(result)[ZSTR_LEN(result)++] = pad_str[i % pad_str_len];

	ZSTR_VAL(result)[ZSTR_LEN(result)] = '\0';

	RETURN_NEW_STR(result);
}
