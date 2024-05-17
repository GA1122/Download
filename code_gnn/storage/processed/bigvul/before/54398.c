static ZIPARCHIVE_METHOD(getArchiveComment)
{
	struct zip *intern;
	zval *self = getThis();
	zend_long flags = 0;
	const char * comment;
	int comment_len = 0;

	if (!self) {
		RETURN_FALSE;
	}

	ZIP_FROM_OBJECT(intern, self);

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "|l", &flags) == FAILURE) {
		return;
	}

	comment = zip_get_archive_comment(intern, &comment_len, (int)flags);
	if(comment==NULL) {
		RETURN_FALSE;
	}
	RETURN_STRINGL((char *)comment, (zend_long)comment_len);
}
