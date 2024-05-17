static ZIPARCHIVE_METHOD(locateName)
{
	struct zip *intern;
	zval *self = getThis();
	zend_long flags = 0;
	zend_long idx = -1;
	zend_string *name;

	if (!self) {
		RETURN_FALSE;
	}

	ZIP_FROM_OBJECT(intern, self);

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "P|l", &name, &flags) == FAILURE) {
		return;
	}

	if (ZSTR_LEN(name) < 1) {
		RETURN_FALSE;
	}

	idx = (zend_long)zip_name_locate(intern, (const char *)ZSTR_VAL(name), flags);

	if (idx >= 0) {
		RETURN_LONG(idx);
	} else {
		RETURN_FALSE;
	}
}
