PHP_FUNCTION(bin2hex)
{
	zend_string *result;
	zend_string *data;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "S", &data) == FAILURE) {
		return;
	}

	result = php_bin2hex((unsigned char *)ZSTR_VAL(data), ZSTR_LEN(data));

	if (!result) {
		RETURN_FALSE;
	}

	RETURN_STR(result);
}
