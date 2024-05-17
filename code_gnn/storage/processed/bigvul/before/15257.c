PHP_FUNCTION(stream_get_contents)
{
	php_stream	*stream;
	zval		*zsrc;
	long		maxlen		= PHP_STREAM_COPY_ALL,
				desiredpos	= -1L;
	long		len;
	char		*contents	= NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r|ll", &zsrc, &maxlen, &desiredpos) == FAILURE) {
		RETURN_FALSE;
	}

	php_stream_from_zval(stream, &zsrc);

	if (desiredpos >= 0) {
		int		seek_res = 0;
		off_t	position;

		position = php_stream_tell(stream);
		if (position >= 0 && desiredpos > position) {
			 
			seek_res = php_stream_seek(stream, desiredpos - position, SEEK_CUR);
		} else if (desiredpos < position)  {
			 
			seek_res = php_stream_seek(stream, desiredpos, SEEK_SET);
		}

		if (seek_res != 0) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING,
				"Failed to seek to position %ld in the stream", desiredpos);
			RETURN_FALSE;
		}
	}

	len = php_stream_copy_to_mem(stream, &contents, maxlen, 0);

	if (contents) {
		if (len > INT_MAX) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "content truncated from %ld to %d bytes", len, INT_MAX);
			len = INT_MAX;
		}
		RETVAL_STRINGL(contents, len, 0);
	} else {
		RETVAL_EMPTY_STRING();
	}
}
