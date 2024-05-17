PHP_FUNCTION(lcfirst)
{
	zend_string  *str;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "S", &str) == FAILURE) {
		return;
	}

	if (!ZSTR_LEN(str)) {
		RETURN_EMPTY_STRING();
	}

	ZVAL_STRINGL(return_value, ZSTR_VAL(str), ZSTR_LEN(str));
	php_lcfirst(Z_STRVAL_P(return_value));
}
