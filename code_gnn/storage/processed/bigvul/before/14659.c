PHP_FUNCTION(str_shuffle)
{
	zend_string *arg;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "S", &arg) == FAILURE) {
		return;
	}

	RETVAL_STRINGL(ZSTR_VAL(arg), ZSTR_LEN(arg));
	if (Z_STRLEN_P(return_value) > 1) {
		php_string_shuffle(Z_STRVAL_P(return_value), (zend_long) Z_STRLEN_P(return_value));
	}
}
