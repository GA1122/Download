static ZIPARCHIVE_METHOD(open)
{
	struct zip *intern;
	int err = 0;
	zend_long flags = 0;
	char *resolved_path;
	zend_string *filename;
	zval *self = getThis();
	ze_zip_object *ze_obj = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "P|l", &filename, &flags) == FAILURE) {
		return;
	}

	if (self) {
		 
		ze_obj = Z_ZIP_P(self);
	}

	if (ZSTR_LEN(filename) == 0) {
		php_error_docref(NULL, E_WARNING, "Empty string as source");
		RETURN_FALSE;
	}

	if (ZIP_OPENBASEDIR_CHECKPATH(ZSTR_VAL(filename))) {
		RETURN_FALSE;
	}

	if (!(resolved_path = expand_filepath(ZSTR_VAL(filename), NULL))) {
		RETURN_FALSE;
	}

	if (ze_obj->za) {
		 
		if (zip_close(ze_obj->za) != 0) {
			php_error_docref(NULL, E_WARNING, "Empty string as source");
			efree(resolved_path);
			RETURN_FALSE;
		}
		ze_obj->za = NULL;
	}
	if (ze_obj->filename) {
		efree(ze_obj->filename);
		ze_obj->filename = NULL;
	}

	intern = zip_open(resolved_path, flags, &err);
	if (!intern || err) {
		efree(resolved_path);
		RETURN_LONG((zend_long)err);
	}
	ze_obj->filename = resolved_path;
	ze_obj->filename_len = strlen(resolved_path);
	ze_obj->za = intern;
	RETURN_TRUE;
}
