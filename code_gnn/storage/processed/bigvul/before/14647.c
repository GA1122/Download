PHP_FUNCTION(setlocale)
{
	zval *args = NULL;
	zval *plocale;
	zend_string *loc;
	char *retval;
	zend_long cat;
	int num_args, i = 0;
	uint32_t idx;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "l+", &cat, &args, &num_args) == FAILURE) {
		return;
	}

#ifdef HAVE_SETLOCALE
	idx = 0;
	while (1) {
		if (Z_TYPE(args[0]) == IS_ARRAY) {
			while (idx < Z_ARRVAL(args[0])->nNumUsed) {
				plocale = &Z_ARRVAL(args[0])->arData[idx].val;
				if (Z_TYPE_P(plocale) != IS_UNDEF) {
					break;
				}
				idx++;
			}
			if (idx >= Z_ARRVAL(args[0])->nNumUsed) {
				break;
			}
		} else {
			plocale = &args[i];
		}

		loc = zval_get_string(plocale);

		if (!strcmp("0", ZSTR_VAL(loc))) {
			zend_string_release(loc);
			loc = NULL;
		} else {
			if (ZSTR_LEN(loc) >= 255) {
				php_error_docref(NULL, E_WARNING, "Specified locale name is too long");
				zend_string_release(loc);
				break;
			}
		}

		retval = php_my_setlocale(cat, loc ? ZSTR_VAL(loc) : NULL);
		zend_update_current_locale();
		if (retval) {
			if (loc) {
				 
				size_t len = strlen(retval);

				BG(locale_changed) = 1;
				if (cat == LC_CTYPE || cat == LC_ALL) {
					if (BG(locale_string)) {
						zend_string_release(BG(locale_string));
					}
					if (len == ZSTR_LEN(loc) && !memcmp(ZSTR_VAL(loc), retval, len)) {
						BG(locale_string) = zend_string_copy(loc);
						RETURN_STR(BG(locale_string));
					} else {
						BG(locale_string) = zend_string_init(retval, len, 0);
						zend_string_release(loc);
						RETURN_STR_COPY(BG(locale_string));
					}
				} else if (len == ZSTR_LEN(loc) && !memcmp(ZSTR_VAL(loc), retval, len)) {
					RETURN_STR(loc);
				}
				zend_string_release(loc);
			}
			RETURN_STRING(retval);
		}
		if (loc) {
			zend_string_release(loc);
		}

		if (Z_TYPE(args[0]) == IS_ARRAY) {
			idx++;
		} else {
			if (++i >= num_args) break;
		}
	}

#endif
	RETURN_FALSE;
}
