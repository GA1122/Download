static ZIPARCHIVE_METHOD(getCommentName)
{
	struct zip *intern;
	zval *self = getThis();
	size_t name_len;
	int idx;
	zend_long flags = 0;
	int comment_len = 0;
	const char * comment;
	char *name;

	if (!self) {
		RETURN_FALSE;
	}

	ZIP_FROM_OBJECT(intern, self);

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s|l",
			&name, &name_len, &flags) == FAILURE) {
		return;
	}
	if (name_len < 1) {
		php_error_docref(NULL, E_NOTICE, "Empty string as entry name");
		RETURN_FALSE;
	}

	idx = zip_name_locate(intern, name, 0);
	if (idx < 0) {
		RETURN_FALSE;
	}

	comment = zip_get_file_comment(intern, idx, &comment_len, (int)flags);
	RETURN_STRINGL((char *)comment, (zend_long)comment_len);
}
