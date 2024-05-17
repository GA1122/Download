PHP_FUNCTION(hex2bin)
{
	zend_string *result, *data;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "S", &data) == FAILURE) {
		return;
	}

	if (ZSTR_LEN(data) % 2 != 0) {
		php_error_docref(NULL, E_WARNING, "Hexadecimal input string must have an even length");
		RETURN_FALSE;
	}

	result = php_hex2bin((unsigned char *)ZSTR_VAL(data), ZSTR_LEN(data));

	if (!result) {
		php_error_docref(NULL, E_WARNING, "Input string must be hexadecimal string");
		RETURN_FALSE;
	}

	RETVAL_STR(result);
}
