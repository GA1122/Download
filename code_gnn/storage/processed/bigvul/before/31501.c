PHP_FUNCTION(radius_put_int)
{
	long type, val;
	radius_descriptor *raddesc;
	zval *z_radh;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rll", &z_radh, &type, &val)
		== FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(raddesc, radius_descriptor *, &z_radh, -1, "rad_handle", le_radius);

	if (rad_put_int(raddesc->radh, type, val) == -1) {
		RETURN_FALSE;
	} else {
		RETURN_TRUE;
	}
}
