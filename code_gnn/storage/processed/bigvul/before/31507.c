PHP_FUNCTION(radius_send_request)
{
	radius_descriptor *raddesc;
	zval *z_radh;
	int res;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &z_radh)
		== FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(raddesc, radius_descriptor *, &z_radh, -1, "rad_handle", le_radius);

	res = rad_send_request(raddesc->radh);
	if (res == -1) {
		RETURN_FALSE;
	} else {
		RETURN_LONG(res);
	}
}
