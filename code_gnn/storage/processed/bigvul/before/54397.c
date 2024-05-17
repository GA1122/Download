static ZIPARCHIVE_METHOD(setArchiveComment)
{
	struct zip *intern;
	zval *self = getThis();
	size_t comment_len;
	char * comment;

	if (!self) {
		RETURN_FALSE;
	}

	ZIP_FROM_OBJECT(intern, self);

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &comment, &comment_len) == FAILURE) {
		return;
	}
	if (zip_set_archive_comment(intern, (const char *)comment, (int)comment_len)) {
		RETURN_FALSE;
	} else {
		RETURN_TRUE;
	}
}
