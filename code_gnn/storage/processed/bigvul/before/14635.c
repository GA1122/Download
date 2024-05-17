PHP_FUNCTION(strtr)
{
	zval *from;
	zend_string *str;
	char *to = NULL;
	size_t to_len = 0;
	int ac = ZEND_NUM_ARGS();

#ifndef FAST_ZPP
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "Sz|s", &str, &from, &to, &to_len) == FAILURE) {
		return;
	}
#else
	ZEND_PARSE_PARAMETERS_START(2, 3)
		Z_PARAM_STR(str)
		Z_PARAM_ZVAL(from)
		Z_PARAM_OPTIONAL
		Z_PARAM_STRING(to, to_len)
	ZEND_PARSE_PARAMETERS_END();
#endif

	if (ac == 2 && Z_TYPE_P(from) != IS_ARRAY) {
		php_error_docref(NULL, E_WARNING, "The second argument is not an array");
		RETURN_FALSE;
	}

	 
	if (ZSTR_LEN(str) == 0) {
		RETURN_EMPTY_STRING();
	}

	if (ac == 2) {
		HashTable *pats = HASH_OF(from);

		if (zend_hash_num_elements(pats) < 1) {
			RETURN_STR_COPY(str);
		} else if (zend_hash_num_elements(pats) == 1) {
			zend_long num_key;
			zend_string *str_key, *replace;
			zval *entry, tmp;

			ZEND_HASH_FOREACH_KEY_VAL(pats, num_key, str_key, entry) {
				ZVAL_UNDEF(&tmp);
				if (UNEXPECTED(!str_key)) {
					ZVAL_LONG(&tmp, num_key);
					convert_to_string(&tmp);
					str_key = Z_STR(tmp);
				}		
				replace = zval_get_string(entry);
				if (ZSTR_LEN(str_key) < 1) {
					RETVAL_STR_COPY(str);
				} else if (ZSTR_LEN(str_key) == 1) {
					RETVAL_STR(php_char_to_str_ex(str,
								ZSTR_VAL(str_key)[0],
								ZSTR_VAL(replace),
								ZSTR_LEN(replace),
								1,
								NULL));
				} else {
					zend_long dummy;
					RETVAL_STR(php_str_to_str_ex(str,
								ZSTR_VAL(str_key), ZSTR_LEN(str_key),
								ZSTR_VAL(replace), ZSTR_LEN(replace), &dummy));
				}
				zend_string_release(replace);
				zval_dtor(&tmp);
				return;
			} ZEND_HASH_FOREACH_END();
		} else {
			php_strtr_array(return_value, str, pats);
		}
	} else {
		convert_to_string_ex(from);

		RETURN_STR(php_strtr_ex(str,
				  Z_STRVAL_P(from),
				  to,
				  MIN(Z_STRLEN_P(from), to_len)));
	}
}
