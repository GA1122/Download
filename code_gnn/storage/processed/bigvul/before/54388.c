static ZIPARCHIVE_METHOD(close)
{
	struct zip *intern;
	zval *self = getThis();
	ze_zip_object *ze_obj;
	int err;

	if (!self) {
		RETURN_FALSE;
	}

	ZIP_FROM_OBJECT(intern, self);

	ze_obj = Z_ZIP_P(self);

	if ((err = zip_close(intern))) {
		php_error_docref(NULL, E_WARNING, "%s", zip_strerror(intern));
		zip_discard(intern);
	}

	efree(ze_obj->filename);
	ze_obj->filename = NULL;
	ze_obj->filename_len = 0;
	ze_obj->za = NULL;

	if (!err) {
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}
}
