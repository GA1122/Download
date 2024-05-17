PHP_FUNCTION(dirname)
{
	char *str;
	zend_string *ret;
	size_t str_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &str, &str_len) == FAILURE) {
		return;
	}

	ret = zend_string_init(str, str_len, 0);
	ZSTR_LEN(ret) = zend_dirname(ZSTR_VAL(ret), str_len);

	RETURN_NEW_STR(ret);
}
