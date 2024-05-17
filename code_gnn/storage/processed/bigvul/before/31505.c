PHP_FUNCTION(radius_put_vendor_int)
{
	long type, vendor, val;
	radius_descriptor *raddesc;
	zval *z_radh;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rlll", &z_radh, &vendor, &type, &val)
		== FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(raddesc, radius_descriptor *, &z_radh, -1, "rad_handle", le_radius);

	if (rad_put_vendor_int(raddesc->radh, vendor, type, val) == -1) {
		RETURN_FALSE;
	} else {
		RETURN_TRUE;
	}
}
