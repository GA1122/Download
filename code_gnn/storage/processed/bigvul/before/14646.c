PHP_FUNCTION(nl2br)
{
	 
	char	*tmp;
	zend_string *str;
	char	*end, *target;
	size_t	repl_cnt = 0;
	zend_bool	is_xhtml = 1;
	zend_string *result;

#ifndef FAST_ZPP
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "S|b", &str, &is_xhtml) == FAILURE) {
		return;
	}
#else
	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_STR(str)
		Z_PARAM_OPTIONAL
		Z_PARAM_BOOL(is_xhtml)
	ZEND_PARSE_PARAMETERS_END();
#endif

	tmp = ZSTR_VAL(str);
	end = ZSTR_VAL(str) + ZSTR_LEN(str);

	 
	while (tmp < end) {
		if (*tmp == '\r') {
			if (*(tmp+1) == '\n') {
				tmp++;
			}
			repl_cnt++;
		} else if (*tmp == '\n') {
			if (*(tmp+1) == '\r') {
				tmp++;
			}
			repl_cnt++;
		}

		tmp++;
	}

	if (repl_cnt == 0) {
		RETURN_STR_COPY(str);
	}

	{
		size_t repl_len = is_xhtml ? (sizeof("<br />") - 1) : (sizeof("<br>") - 1);

		result = zend_string_alloc(repl_cnt * repl_len + ZSTR_LEN(str), 0);
		target = ZSTR_VAL(result);
	}

	tmp = ZSTR_VAL(str);
	while (tmp < end) {
		switch (*tmp) {
			case '\r':
			case '\n':
				*target++ = '<';
				*target++ = 'b';
				*target++ = 'r';

				if (is_xhtml) {
					*target++ = ' ';
					*target++ = '/';
				}

				*target++ = '>';

				if ((*tmp == '\r' && *(tmp+1) == '\n') || (*tmp == '\n' && *(tmp+1) == '\r')) {
					*target++ = *tmp++;
				}
				 
			default:
				*target++ = *tmp;
		}

		tmp++;
	}

	*target = '\0';

	RETURN_NEW_STR(result);
}
