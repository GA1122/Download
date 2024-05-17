static PHP_NAMED_FUNCTION(zif_zip_entry_open)
{
	zval * zip;
	zval * zip_entry;
	char *mode = NULL;
	size_t mode_len = 0;
	zip_read_rsrc * zr_rsrc;
	zip_rsrc *z_rsrc;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "rr|s", &zip, &zip_entry, &mode, &mode_len) == FAILURE) {
		return;
	}

	if ((zr_rsrc = (zip_read_rsrc *)zend_fetch_resource(Z_RES_P(zip_entry), le_zip_entry_name, le_zip_entry)) == NULL) {
		RETURN_FALSE;
	}

	if ((z_rsrc = (zip_rsrc *)zend_fetch_resource(Z_RES_P(zip), le_zip_dir_name, le_zip_dir)) == NULL) {
		RETURN_FALSE;
	}

	if (zr_rsrc->zf != NULL) {
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}
}
