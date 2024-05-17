PHP_FUNCTION(radius_strerror)
{
	char *msg;
	radius_descriptor *raddesc;
	zval *z_radh;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &z_radh) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(raddesc, radius_descriptor *, &z_radh, -1, "rad_handle", le_radius);
	msg = (char *)rad_strerror(raddesc->radh);
	RETURN_STRINGL(msg, strlen(msg), 1);
}
