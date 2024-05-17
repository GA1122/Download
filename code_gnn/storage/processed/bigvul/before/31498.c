PHP_FUNCTION(radius_add_server)
{
	char *hostname, *secret;
	int hostname_len, secret_len;
	long  port, timeout, maxtries;
	radius_descriptor *raddesc;
	zval *z_radh;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rslsll", &z_radh,
		&hostname, &hostname_len,
		&port,
		&secret, &secret_len,
		&timeout, &maxtries) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(raddesc, radius_descriptor *, &z_radh, -1, "rad_handle", le_radius);

	if (rad_add_server(raddesc->radh, hostname, port, secret, timeout, maxtries) == -1) {
		RETURN_FALSE;
	} else {
		RETURN_TRUE;
	}
}
