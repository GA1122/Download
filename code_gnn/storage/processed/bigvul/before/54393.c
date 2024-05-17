static ZIPARCHIVE_METHOD(statName)
{
	struct zip *intern;
	zval *self = getThis();
	zend_long flags = 0;
	struct zip_stat sb;
	zend_string *name;

	if (!self) {
		RETURN_FALSE;
	}

	ZIP_FROM_OBJECT(intern, self);

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "P|l", &name, &flags) == FAILURE) {
		return;
	}

	PHP_ZIP_STAT_PATH(intern, ZSTR_VAL(name), ZSTR_LEN(name), flags, sb);

	RETURN_SB(&sb);
}
