PHP_FUNCTION(radius_put_addr)
{
	int addrlen;
	long type;
	char	*addr;
	radius_descriptor *raddesc;
	zval *z_radh;
	struct in_addr intern_addr;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rls", &z_radh, &type, &addr, &addrlen)
		== FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(raddesc, radius_descriptor *, &z_radh, -1, "rad_handle", le_radius);

	if (inet_aton(addr, &intern_addr) == 0) {
		zend_error(E_ERROR, "Error converting Address");
		RETURN_FALSE;
	}

	if (rad_put_addr(raddesc->radh, type, intern_addr) == -1) {
		RETURN_FALSE;
	} else {
		RETURN_TRUE;
	}
}
