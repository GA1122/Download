PHP_FUNCTION(addcslashes)
{
	zend_string *str, *what;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "SS", &str, &what) == FAILURE) {
		return;
	}

	if (ZSTR_LEN(str) == 0) {
		RETURN_EMPTY_STRING();
	}

	if (ZSTR_LEN(what) == 0) {
		RETURN_STRINGL(ZSTR_VAL(str), ZSTR_LEN(str));
	}

	RETURN_STR(php_addcslashes(str, 0, ZSTR_VAL(what), ZSTR_LEN(what)));
}
