PHP_FUNCTION(radius_put_vendor_attr)
{
	long type, vendor;
	int len;
	char *data;
	radius_descriptor *raddesc;
	zval *z_radh;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rlls", &z_radh, &vendor, &type,
		&data, &len) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(raddesc, radius_descriptor *, &z_radh, -1, "rad_handle", le_radius);

	if (rad_put_vendor_attr(raddesc->radh, vendor, type, data, len) == -1) {
		RETURN_FALSE;
	} else {
		RETURN_TRUE;
	}
}
