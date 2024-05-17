PHP_FUNCTION(radius_config)
{
	char *filename;
	int filename_len;
	radius_descriptor *raddesc;
	zval *z_radh;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs", &z_radh, &filename, &filename_len) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(raddesc, radius_descriptor *, &z_radh, -1, "rad_handle", le_radius);

	if (rad_config(raddesc->radh, filename) == -1) {
		RETURN_FALSE;
	} else {
		RETURN_TRUE;
	}
}
