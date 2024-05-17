PHP_FUNCTION(str_split)
{
	zend_string *str;
	zend_long split_length = 1;
	char *p;
	size_t n_reg_segments;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "S|l", &str, &split_length) == FAILURE) {
		return;
	}

	if (split_length <= 0) {
		php_error_docref(NULL, E_WARNING, "The length of each segment must be greater than zero");
		RETURN_FALSE;
	}


	if (0 == ZSTR_LEN(str) || (size_t)split_length >= ZSTR_LEN(str)) {
		array_init_size(return_value, 1);
		add_next_index_stringl(return_value, ZSTR_VAL(str), ZSTR_LEN(str));
		return;
	}

	array_init_size(return_value, (uint32_t)(((ZSTR_LEN(str) - 1) / split_length) + 1));

	n_reg_segments = ZSTR_LEN(str) / split_length;
	p = ZSTR_VAL(str);

	while (n_reg_segments-- > 0) {
		add_next_index_stringl(return_value, p, split_length);
		p += split_length;
	}

	if (p != (ZSTR_VAL(str) + ZSTR_LEN(str))) {
		add_next_index_stringl(return_value, p, (ZSTR_VAL(str) + ZSTR_LEN(str) - p));
	}
}
