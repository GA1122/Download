static ZIPARCHIVE_METHOD(getExternalAttributesIndex)
{
	struct zip *intern;
	zval *self = getThis(), *z_opsys, *z_attr;
	zend_long index, flags=0;
	zip_uint8_t opsys;
	zip_uint32_t attr;
	struct zip_stat sb;

	if (!self) {
		RETURN_FALSE;
	}

	ZIP_FROM_OBJECT(intern, self);

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "lz/z/|l",
			&index, &z_opsys, &z_attr, &flags) == FAILURE) {
		return;
	}

	PHP_ZIP_STAT_INDEX(intern, index, 0, sb);
	if (zip_file_get_external_attributes(intern, (zip_uint64_t)index,
			(zip_flags_t)flags, &opsys, &attr) < 0) {
		RETURN_FALSE;
	}
	zval_dtor(z_opsys);
	ZVAL_LONG(z_opsys, opsys);
	zval_dtor(z_attr);
	ZVAL_LONG(z_attr, attr);
	RETURN_TRUE;
}
