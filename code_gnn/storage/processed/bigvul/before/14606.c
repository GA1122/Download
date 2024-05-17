PHP_FUNCTION(strcoll)
{
	zend_string *s1, *s2;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "SS", &s1, &s2) == FAILURE) {
		return;
	}

	RETURN_LONG(strcoll((const char *) ZSTR_VAL(s1),
	                    (const char *) ZSTR_VAL(s2)));
}
