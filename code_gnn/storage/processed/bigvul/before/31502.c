PHP_FUNCTION(radius_put_attr)
{
	long type;
	int len;
	char *data;
	radius_descriptor *raddesc;
	zval *z_radh;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rls", &z_radh, &type, &data, &len)
		== FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(raddesc, radius_descriptor *, &z_radh, -1, "rad_handle", le_radius);

	if (rad_put_attr(raddesc->radh, type, data, len) == -1) {
		RETURN_FALSE;
	} else {
		RETURN_TRUE;
	}

}
