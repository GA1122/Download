static ZIPARCHIVE_METHOD(getExternalAttributesName)
{
	struct zip *intern;
	zval *self = getThis(), *z_opsys, *z_attr;
	size_t name_len;
	char *name;
	zend_long flags=0;
	zip_uint8_t opsys;
	zip_uint32_t attr;
	zip_int64_t idx;

	if (!self) {
		RETURN_FALSE;
	}

	ZIP_FROM_OBJECT(intern, self);

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "sz/z/|l",
			&name, &name_len, &z_opsys, &z_attr, &flags) == FAILURE) {
		return;
	}

	if (name_len < 1) {
		php_error_docref(NULL, E_NOTICE, "Empty string as entry name");
	}

	idx = zip_name_locate(intern, name, 0);
	if (idx < 0) {
		RETURN_FALSE;
	}
	if (zip_file_get_external_attributes(intern, idx,
			(zip_flags_t)flags, &opsys, &attr) < 0) {
		RETURN_FALSE;
	}
	zval_ptr_dtor(z_opsys);
	ZVAL_LONG(z_opsys, opsys);
	zval_ptr_dtor(z_attr);
	ZVAL_LONG(z_attr, attr);
	RETURN_TRUE;
}
