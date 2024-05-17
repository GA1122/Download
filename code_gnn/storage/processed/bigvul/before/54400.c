static ZIPARCHIVE_METHOD(setCommentIndex)
{
	struct zip *intern;
	zval *self = getThis();
	zend_long index;
	size_t comment_len;
	char * comment;
	struct zip_stat sb;

	if (!self) {
		RETURN_FALSE;
	}

	ZIP_FROM_OBJECT(intern, self);

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "ls",
			&index, &comment, &comment_len) == FAILURE) {
		return;
	}

	PHP_ZIP_STAT_INDEX(intern, index, 0, sb);
	PHP_ZIP_SET_FILE_COMMENT(intern, index, comment, comment_len);
}
