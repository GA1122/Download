PHP_FUNCTION(radius_create_request)
{
	long code;
	radius_descriptor *raddesc;
	zval *z_radh;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rl", &z_radh, &code) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(raddesc, radius_descriptor *, &z_radh, -1, "rad_handle", le_radius);

	if (rad_create_request(raddesc->radh, code) == -1) {
		RETURN_FALSE;
	} else {
		RETURN_TRUE;
	}
}
