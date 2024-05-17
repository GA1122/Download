PHP_FUNCTION(radius_request_authenticator)
{
	radius_descriptor *raddesc;
	ssize_t res;
	char buf[LEN_AUTH];
	zval *z_radh;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &z_radh) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(raddesc, radius_descriptor *, &z_radh, -1, "rad_handle", le_radius);

	res = rad_request_authenticator(raddesc->radh, buf, sizeof buf);
	if (res == -1) {
		RETURN_FALSE;
	} else {
		RETURN_STRINGL(buf, res, 1);
	}
}
