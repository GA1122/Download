static ZIPARCHIVE_METHOD(setExternalAttributesIndex)
{
	struct zip *intern;
	zval *self = getThis();
	zend_long index, flags=0, opsys, attr;
	struct zip_stat sb;

	if (!self) {
		RETURN_FALSE;
	}

	ZIP_FROM_OBJECT(intern, self);

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "lll|l",
			&index, &opsys, &attr, &flags) == FAILURE) {
		return;
	}

	PHP_ZIP_STAT_INDEX(intern, index, 0, sb);
	if (zip_file_set_external_attributes(intern, (zip_uint64_t)index,
			(zip_flags_t)flags, (zip_uint8_t)(opsys&0xff), (zip_uint32_t)attr) < 0) {
		RETURN_FALSE;
	}
	RETURN_TRUE;
}
