static ZIPARCHIVE_METHOD(setExternalAttributesName)
{
	struct zip *intern;
	zval *self = getThis();
	size_t name_len;
	char *name;
	zend_long flags=0, opsys, attr;
	zip_int64_t idx;

	if (!self) {
		RETURN_FALSE;
	}

	ZIP_FROM_OBJECT(intern, self);

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "sll|l",
			&name, &name_len, &opsys, &attr, &flags) == FAILURE) {
		return;
	}

	if (name_len < 1) {
		php_error_docref(NULL, E_NOTICE, "Empty string as entry name");
	}

	idx = zip_name_locate(intern, name, 0);
	if (idx < 0) {
		RETURN_FALSE;
	}
	if (zip_file_set_external_attributes(intern, idx, (zip_flags_t)flags,
			(zip_uint8_t)(opsys&0xff), (zip_uint32_t)attr) < 0) {
		RETURN_FALSE;
	}
	RETURN_TRUE;
}
