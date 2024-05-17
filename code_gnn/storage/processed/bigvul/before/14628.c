PHP_FUNCTION(substr_replace)
{
	zval *str;
	zval *from;
	zval *len = NULL;
	zval *repl;
	zend_long l = 0;
	zend_long f;
	int argc = ZEND_NUM_ARGS();
	zend_string *result;
	HashPosition from_idx, repl_idx, len_idx;
	zval *tmp_str = NULL, *tmp_from = NULL, *tmp_repl = NULL, *tmp_len= NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "zzz|z/", &str, &repl, &from, &len) == FAILURE) {
		return;
	}

	if (Z_TYPE_P(str) != IS_ARRAY) {
		convert_to_string_ex(str);
	}
	if (Z_TYPE_P(repl) != IS_ARRAY) {
		convert_to_string_ex(repl);
	}
	if (Z_TYPE_P(from) != IS_ARRAY) {
		convert_to_long_ex(from);
	}

	if (argc > 3) {
		if (Z_TYPE_P(len) != IS_ARRAY) {
			l = zval_get_long(len);
		}
	} else {
		if (Z_TYPE_P(str) != IS_ARRAY) {
			l = Z_STRLEN_P(str);
		}
	}

	if (Z_TYPE_P(str) == IS_STRING) {
		if (
			(argc == 3 && Z_TYPE_P(from) == IS_ARRAY) ||
			(argc == 4 && Z_TYPE_P(from) != Z_TYPE_P(len))
		) {
			php_error_docref(NULL, E_WARNING, "'from' and 'len' should be of same type - numerical or array ");
			RETURN_STR_COPY(Z_STR_P(str));
		}
		if (argc == 4 && Z_TYPE_P(from) == IS_ARRAY) {
			if (zend_hash_num_elements(Z_ARRVAL_P(from)) != zend_hash_num_elements(Z_ARRVAL_P(len))) {
				php_error_docref(NULL, E_WARNING, "'from' and 'len' should have the same number of elements");
				RETURN_STR_COPY(Z_STR_P(str));
			}
		}
	}

	if (Z_TYPE_P(str) != IS_ARRAY) {
		if (Z_TYPE_P(from) != IS_ARRAY) {
			size_t repl_len = 0;

			f = Z_LVAL_P(from);

			 
			if (f < 0) {
				f = (zend_long)Z_STRLEN_P(str) + f;
				if (f < 0) {
					f = 0;
				}
			} else if (f > Z_STRLEN_P(str)) {
				f = Z_STRLEN_P(str);
			}
			 
			if (l < 0) {
				l = ((zend_long)Z_STRLEN_P(str) - f) + l;
				if (l < 0) {
					l = 0;
				}
			}

			if (l > Z_STRLEN_P(str) || (l < 0 && (size_t)(-l) > Z_STRLEN_P(str))) {
				l = Z_STRLEN_P(str);
			}

			if ((f + l) > (zend_long)Z_STRLEN_P(str)) {
				l = Z_STRLEN_P(str) - f;
			}
			if (Z_TYPE_P(repl) == IS_ARRAY) {
				repl_idx = 0;
				while (repl_idx < Z_ARRVAL_P(repl)->nNumUsed) {
					tmp_repl = &Z_ARRVAL_P(repl)->arData[repl_idx].val;
					if (Z_TYPE_P(tmp_repl) != IS_UNDEF) {
						break;
					}
					repl_idx++;
				}
				if (repl_idx < Z_ARRVAL_P(repl)->nNumUsed) {
					convert_to_string_ex(tmp_repl);
					repl_len = Z_STRLEN_P(tmp_repl);
				}
			} else {
				repl_len = Z_STRLEN_P(repl);
			}

			result = zend_string_alloc(Z_STRLEN_P(str) - l + repl_len, 0);

			memcpy(ZSTR_VAL(result), Z_STRVAL_P(str), f);
			if (repl_len) {
				memcpy((ZSTR_VAL(result) + f), (Z_TYPE_P(repl) == IS_ARRAY ? Z_STRVAL_P(tmp_repl) : Z_STRVAL_P(repl)), repl_len);
			}
			memcpy((ZSTR_VAL(result) + f + repl_len), Z_STRVAL_P(str) + f + l, Z_STRLEN_P(str) - f - l);
			ZSTR_VAL(result)[ZSTR_LEN(result)] = '\0';
			RETURN_NEW_STR(result);
		} else {
			php_error_docref(NULL, E_WARNING, "Functionality of 'from' and 'len' as arrays is not implemented");
			RETURN_STR_COPY(Z_STR_P(str));
		}
	} else {  
		zend_string *str_index = NULL;
		size_t result_len;
		zend_ulong num_index;

		array_init(return_value);

		from_idx = len_idx = repl_idx = 0;

		ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(str), num_index, str_index, tmp_str) {
			zend_string *orig_str = zval_get_string(tmp_str);

			if (Z_TYPE_P(from) == IS_ARRAY) {
				while (from_idx < Z_ARRVAL_P(from)->nNumUsed) {
					tmp_from = &Z_ARRVAL_P(from)->arData[from_idx].val;
					if (Z_TYPE_P(tmp_from) != IS_UNDEF) {
						break;
					}
					from_idx++;
				}
				if (from_idx < Z_ARRVAL_P(from)->nNumUsed) {
					f = zval_get_long(tmp_from);

					if (f < 0) {
						f = (zend_long)ZSTR_LEN(orig_str) + f;
						if (f < 0) {
							f = 0;
						}
					} else if (f > (zend_long)ZSTR_LEN(orig_str)) {
						f = ZSTR_LEN(orig_str);
					}
					from_idx++;
				} else {
					f = 0;
				}
			} else {
				f = Z_LVAL_P(from);
				if (f < 0) {
					f = (zend_long)ZSTR_LEN(orig_str) + f;
					if (f < 0) {
						f = 0;
					}
				} else if (f > (zend_long)ZSTR_LEN(orig_str)) {
					f = ZSTR_LEN(orig_str);
				}
			}

			if (argc > 3 && Z_TYPE_P(len) == IS_ARRAY) {
				while (len_idx < Z_ARRVAL_P(len)->nNumUsed) {
					tmp_len = &Z_ARRVAL_P(len)->arData[len_idx].val;
					if (Z_TYPE_P(tmp_len) != IS_UNDEF) {
						break;
					}
					len_idx++;
				}
				if (len_idx < Z_ARRVAL_P(len)->nNumUsed) {
					l = zval_get_long(tmp_len);
					len_idx++;
				} else {
					l = ZSTR_LEN(orig_str);
				}
			} else if (argc > 3) {
				l = Z_LVAL_P(len);
			} else {
				l = ZSTR_LEN(orig_str);
			}

			if (l < 0) {
				l = (ZSTR_LEN(orig_str) - f) + l;
				if (l < 0) {
					l = 0;
				}
			}

			if ((f + l) > (zend_long)ZSTR_LEN(orig_str)) {
				l = ZSTR_LEN(orig_str) - f;
			}

			result_len = ZSTR_LEN(orig_str) - l;

			if (Z_TYPE_P(repl) == IS_ARRAY) {
				while (repl_idx < Z_ARRVAL_P(repl)->nNumUsed) {
					tmp_repl = &Z_ARRVAL_P(repl)->arData[repl_idx].val;
					if (Z_TYPE_P(tmp_repl) != IS_UNDEF) {
						break;
					}
					repl_idx++;
				}
				if (repl_idx < Z_ARRVAL_P(repl)->nNumUsed) {
					zend_string *repl_str = zval_get_string(tmp_repl);

					result_len += ZSTR_LEN(repl_str);
					repl_idx++;
					result = zend_string_alloc(result_len, 0);

					memcpy(ZSTR_VAL(result), ZSTR_VAL(orig_str), f);
					memcpy((ZSTR_VAL(result) + f), ZSTR_VAL(repl_str), ZSTR_LEN(repl_str));
					memcpy((ZSTR_VAL(result) + f + ZSTR_LEN(repl_str)), ZSTR_VAL(orig_str) + f + l, ZSTR_LEN(orig_str) - f - l);
					zend_string_release(repl_str);
				} else {
					result = zend_string_alloc(result_len, 0);

					memcpy(ZSTR_VAL(result), ZSTR_VAL(orig_str), f);
					memcpy((ZSTR_VAL(result) + f), ZSTR_VAL(orig_str) + f + l, ZSTR_LEN(orig_str) - f - l);
				}
			} else {
				result_len += Z_STRLEN_P(repl);

				result = zend_string_alloc(result_len, 0);

				memcpy(ZSTR_VAL(result), ZSTR_VAL(orig_str), f);
				memcpy((ZSTR_VAL(result) + f), Z_STRVAL_P(repl), Z_STRLEN_P(repl));
				memcpy((ZSTR_VAL(result) + f + Z_STRLEN_P(repl)), ZSTR_VAL(orig_str) + f + l, ZSTR_LEN(orig_str) - f - l);
			}

			ZSTR_VAL(result)[ZSTR_LEN(result)] = '\0';

			if (str_index) {
				zval tmp;

				ZVAL_NEW_STR(&tmp, result);
				zend_symtable_update(Z_ARRVAL_P(return_value), str_index, &tmp);
			} else {
				add_index_str(return_value, num_index, result);
			}

			zend_string_release(orig_str);
		} ZEND_HASH_FOREACH_END();
	}  
}
