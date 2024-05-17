PHP_FUNCTION(str_rot13)
{
	zend_string *arg;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "S", &arg) == FAILURE) {
		return;
	}

	if (ZSTR_LEN(arg) == 0) {
		RETURN_EMPTY_STRING();
	} else {
		RETURN_STR(php_strtr_ex(arg, rot13_from, rot13_to, 52));
	}
}
