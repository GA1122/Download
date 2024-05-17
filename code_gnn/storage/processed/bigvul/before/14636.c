PHP_FUNCTION(strrev)
{
	zend_string *str;
	char *e, *p;
	zend_string *n;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "S", &str) == FAILURE) {
		return;
	}

	n = zend_string_alloc(ZSTR_LEN(str), 0);
	p = ZSTR_VAL(n);

	e = ZSTR_VAL(str) + ZSTR_LEN(str);

	while (--e >= ZSTR_VAL(str)) {
		*p++ = *e;
	}

	*p = '\0';

	RETVAL_NEW_STR(n);
}
