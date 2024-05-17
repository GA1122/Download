PHP_FUNCTION(quotemeta)
{
	zend_string *old;
	char *old_end;
	char *p, *q;
	char c;
	zend_string *str;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "S", &old) == FAILURE) {
		return;
	}

	old_end = ZSTR_VAL(old) + ZSTR_LEN(old);

	if (ZSTR_VAL(old) == old_end) {
		RETURN_FALSE;
	}

	str = zend_string_alloc(2 * ZSTR_LEN(old), 0);

	for (p = ZSTR_VAL(old), q = ZSTR_VAL(str); p != old_end; p++) {
		c = *p;
		switch (c) {
			case '.':
			case '\\':
			case '+':
			case '*':
			case '?':
			case '[':
			case '^':
			case ']':
			case '$':
			case '(':
			case ')':
				*q++ = '\\';
				 
			default:
				*q++ = c;
		}
	}

	*q = '\0';

	RETURN_NEW_STR(zend_string_truncate(str, q - ZSTR_VAL(str), 0));
}
