static PHP_NAMED_FUNCTION(zif_zip_entry_close)
{
	zval * zip_entry;
	zip_read_rsrc * zr_rsrc;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &zip_entry) == FAILURE) {
		return;
	}

	if ((zr_rsrc = (zip_read_rsrc *)zend_fetch_resource(Z_RES_P(zip_entry), le_zip_entry_name, le_zip_entry)) == NULL) {
		RETURN_FALSE;
	}

	RETURN_BOOL(SUCCESS == zend_list_close(Z_RES_P(zip_entry)));
}
