PHP_FUNCTION(strtok)
{
	zend_string *str, *tok = NULL;
	char *token;
	char *token_end;
	char *p;
	char *pe;
	size_t skipped = 0;

#ifndef FAST_ZPP
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "S|S", &str, &tok) == FAILURE) {
		return;
	}
#else
	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_STR(str)
		Z_PARAM_OPTIONAL
		Z_PARAM_STR(tok)
	ZEND_PARSE_PARAMETERS_END();
#endif

	if (ZEND_NUM_ARGS() == 1) {
		tok = str;
	} else {
		zval_ptr_dtor(&BG(strtok_zval));
		ZVAL_STRINGL(&BG(strtok_zval), ZSTR_VAL(str), ZSTR_LEN(str));
		BG(strtok_last) = BG(strtok_string) = Z_STRVAL(BG(strtok_zval));
		BG(strtok_len) = ZSTR_LEN(str);
	}

	p = BG(strtok_last);  
	pe = BG(strtok_string) + BG(strtok_len);

	if (!p || p >= pe) {
		RETURN_FALSE;
	}

	token = ZSTR_VAL(tok);
	token_end = token + ZSTR_LEN(tok);

	while (token < token_end) {
		STRTOK_TABLE(token++) = 1;
	}

	 
	while (STRTOK_TABLE(p)) {
		if (++p >= pe) {
			 
			BG(strtok_last) = NULL;
			RETVAL_FALSE;
			goto restore;
		}
		skipped++;
	}

	 
	while (++p < pe) {
		if (STRTOK_TABLE(p)) {
			goto return_token;
		}
	}

	if (p - BG(strtok_last)) {
return_token:
		RETVAL_STRINGL(BG(strtok_last) + skipped, (p - BG(strtok_last)) - skipped);
		BG(strtok_last) = p + 1;
	} else {
		RETVAL_FALSE;
		BG(strtok_last) = NULL;
	}

	 
restore:
	token = ZSTR_VAL(tok);

	while (token < token_end) {
		STRTOK_TABLE(token++) = 0;
	}
}
