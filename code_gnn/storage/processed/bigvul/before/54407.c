static ZIPARCHIVE_METHOD(setCompressionName)
 {
	struct zip *intern;
	zval *this = getThis();
	size_t name_len;
	char *name;
	zip_int64_t idx;
	zend_long comp_method, comp_flags = 0;

	if (!this) {
		RETURN_FALSE;
	}

	ZIP_FROM_OBJECT(intern, this);

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "sl|l",
			&name, &name_len, &comp_method, &comp_flags) == FAILURE) {
		return;
	}

	if (name_len < 1) {
		php_error_docref(NULL, E_NOTICE, "Empty string as entry name");
	}

	idx = zip_name_locate(intern, name, 0);
	if (idx < 0) {
		RETURN_FALSE;
	}

	if (zip_set_file_compression(intern, (zip_uint64_t)idx,
			(zip_int32_t)comp_method, (zip_uint32_t)comp_flags) != 0) {
		RETURN_FALSE;
	}
	RETURN_TRUE;
}
