PHP_FUNCTION(radius_cvt_string)
{
	const void *data;
	char *val;
	int len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &data, &len)
		== FAILURE) {
		return;
	}

	val = rad_cvt_string(data, len);
	if (val == NULL) RETURN_FALSE;
	RETVAL_STRINGL(val, strlen(val), 1);
	free(val);
	return;
}
