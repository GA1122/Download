PHP_FUNCTION(radius_put_vendor_string)
{
	char *str;
	int str_len;
	long type, vendor;
	radius_descriptor *raddesc;
	zval *z_radh;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rlls", &z_radh, &vendor, &type, &str, &str_len)
		== FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(raddesc, radius_descriptor *, &z_radh, -1, "rad_handle", le_radius);

	if (rad_put_vendor_string(raddesc->radh, vendor, type, str) == -1) {
		RETURN_FALSE;
	} else {
		RETURN_TRUE;
	}
}
