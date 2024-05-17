PHP_FUNCTION(radius_cvt_int)
{
	const void *data;
	int len, val;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &data, &len)
		== FAILURE) {
		return;
	}

	val = rad_cvt_int(data);
	RETURN_LONG(val);
}
