static ZIPARCHIVE_METHOD(addFile)
{
	struct zip *intern;
	zval *self = getThis();
	char *entry_name = NULL;
	size_t entry_name_len = 0;
	zend_long offset_start = 0, offset_len = 0;
	zend_string *filename;

	if (!self) {
		RETURN_FALSE;
	}

	ZIP_FROM_OBJECT(intern, self);

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "P|sll",
			&filename, &entry_name, &entry_name_len, &offset_start, &offset_len) == FAILURE) {
		return;
	}

	if (ZSTR_LEN(filename) == 0) {
		php_error_docref(NULL, E_NOTICE, "Empty string as filename");
		RETURN_FALSE;
	}

	if (entry_name_len == 0) {
		entry_name = ZSTR_VAL(filename);
		entry_name_len = ZSTR_LEN(filename);
	}

	if (php_zip_add_file(intern, ZSTR_VAL(filename), ZSTR_LEN(filename), entry_name, entry_name_len, 0, 0) < 0) {
		RETURN_FALSE;
	} else {
		RETURN_TRUE;
	}
}
