PHP_FUNCTION(radius_get_attr)
{
	radius_descriptor *raddesc;
	int res;
	const void *data;
	size_t len;
	zval *z_radh;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &z_radh) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(raddesc, radius_descriptor *, &z_radh, -1, "rad_handle", le_radius);

	res = rad_get_attr(raddesc->radh, &data, &len);
	if (res == -1) {
		RETURN_FALSE;
	} else {
		if (res > 0) {

			array_init(return_value);
			add_assoc_long(return_value, "attr", res);
			add_assoc_stringl(return_value, "data", (char *) data, len, 1);
			return;
		}
		RETURN_LONG(res);
	}
}
