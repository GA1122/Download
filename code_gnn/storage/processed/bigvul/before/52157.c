PHPAPI PHP_FUNCTION(fgetss)
{
	zval *fd;
	long bytes = 0;
	size_t len = 0;
	size_t actual_len, retval_len;
	char *buf = NULL, *retval;
	php_stream *stream;
	char *allowed_tags=NULL;
	int allowed_tags_len=0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r|ls", &fd, &bytes, &allowed_tags, &allowed_tags_len) == FAILURE) {
		RETURN_FALSE;
	}

	PHP_STREAM_TO_ZVAL(stream, &fd);

	if (ZEND_NUM_ARGS() >= 2) {
		if (bytes <= 0) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Length parameter must be greater than 0");
			RETURN_FALSE;
		}

		len = (size_t) bytes;
		buf = safe_emalloc(sizeof(char), (len + 1), 0);
		 
		memset(buf, 0, len + 1);
	}

	if ((retval = php_stream_get_line(stream, buf, len, &actual_len)) == NULL)	{
		if (buf != NULL) {
			efree(buf);
		}
		RETURN_FALSE;
	}

	retval_len = php_strip_tags(retval, actual_len, &stream->fgetss_state, allowed_tags, allowed_tags_len);

	RETURN_STRINGL(retval, retval_len, 0);
}
