PHP_FUNCTION(radius_server_secret)
{
	char *secret;
	radius_descriptor *raddesc;
	zval *z_radh;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &z_radh) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(raddesc, radius_descriptor *, &z_radh, -1, "rad_handle", le_radius);
	secret = (char *)rad_server_secret(raddesc->radh);

	if (secret) {
		RETURN_STRINGL(secret, strlen(secret), 1);
	}

	RETURN_FALSE;
}
