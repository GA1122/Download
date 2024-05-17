PHP_FUNCTION(checkdate)
{
	long m, d, y;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lll", &m, &d, &y) == FAILURE) {
		RETURN_FALSE;
	}

	if (y < 1 || y > 32767 || !timelib_valid_date(y, m, d)) {
		RETURN_FALSE;
	}
	RETURN_TRUE;	 
}
