static ZIPARCHIVE_METHOD(unchangeAll)
{
	struct zip *intern;
	zval *self = getThis();

	if (!self) {
		RETURN_FALSE;
	}

	ZIP_FROM_OBJECT(intern, self);

	if (zip_unchange_all(intern) != 0) {
		RETURN_FALSE;
	} else {
		RETURN_TRUE;
	}
}
