PHP_FUNCTION(str_word_count)
{
	zend_string *str;
	char *char_list = NULL, *p, *e, *s, ch[256];
	size_t char_list_len = 0, word_count = 0;
	zend_long type = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "S|ls", &str, &type, &char_list, &char_list_len) == FAILURE) {
		return;
	}

	switch(type) {
		case 1:
		case 2:
			array_init(return_value);
			if (!ZSTR_LEN(str)) {
				return;
			}
			break;
		case 0:
			if (!ZSTR_LEN(str)) {
				RETURN_LONG(0);
			}
			 
			break;
		default:
			php_error_docref(NULL, E_WARNING, "Invalid format value " ZEND_LONG_FMT, type);
			RETURN_FALSE;
	}

	if (char_list) {
		php_charmask((unsigned char *)char_list, char_list_len, ch);
	}

	p = ZSTR_VAL(str);
	e = ZSTR_VAL(str) + ZSTR_LEN(str);

	 
	if ((*p == '\'' && (!char_list || !ch['\''])) || (*p == '-' && (!char_list || !ch['-']))) {
		p++;
	}
	 
	if (*(e - 1) == '-' && (!char_list || !ch['-'])) {
		e--;
	}

	while (p < e) {
		s = p;
		while (p < e && (isalpha((unsigned char)*p) || (char_list && ch[(unsigned char)*p]) || *p == '\'' || *p == '-')) {
			p++;
		}
		if (p > s) {
			switch (type)
			{
				case 1:
					add_next_index_stringl(return_value, s, p - s);
					break;
				case 2:
					add_index_stringl(return_value, (s - ZSTR_VAL(str)), s, p - s);
					break;
				default:
					word_count++;
					break;
			}
		}
		p++;
	}

	if (!type) {
		RETURN_LONG(word_count);
	}
}
