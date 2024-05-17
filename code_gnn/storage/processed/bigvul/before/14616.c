PHP_FUNCTION(basename)
{
	char *string, *suffix = NULL;
	size_t   string_len, suffix_len = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s|s", &string, &string_len, &suffix, &suffix_len) == FAILURE) {
		return;
	}

	RETURN_STR(php_basename(string, string_len, suffix, suffix_len));
}
