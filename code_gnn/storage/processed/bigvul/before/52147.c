PHP_FUNCTION(file_get_contents)
{
	char *filename;
	int filename_len;
	char *contents;
	zend_bool use_include_path = 0;
	php_stream *stream;
	long len;
	long offset = -1;
	long maxlen = PHP_STREAM_COPY_ALL;
	zval *zcontext = NULL;
	php_stream_context *context = NULL;

	 
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "p|br!ll", &filename, &filename_len, &use_include_path, &zcontext, &offset, &maxlen) == FAILURE) {
		return;
	}

	if (ZEND_NUM_ARGS() == 5 && maxlen < 0) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "length must be greater than or equal to zero");
		RETURN_FALSE;
	}

	context = php_stream_context_from_zval(zcontext, 0);

	stream = php_stream_open_wrapper_ex(filename, "rb",
				(use_include_path ? USE_PATH : 0) | REPORT_ERRORS,
				NULL, context);
	if (!stream) {
		RETURN_FALSE;
	}

	if (offset > 0 && php_stream_seek(stream, offset, SEEK_SET) < 0) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Failed to seek to position %ld in the stream", offset);
		php_stream_close(stream);
		RETURN_FALSE;
	}

	if ((len = php_stream_copy_to_mem(stream, &contents, maxlen, 0)) > 0) {
		if (len > INT_MAX) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "content truncated from %ld to %d bytes", len, INT_MAX);
			len = INT_MAX;
		}
		RETVAL_STRINGL(contents, len, 0);
	} else if (len == 0) {
		RETVAL_EMPTY_STRING();
	} else {
		RETVAL_FALSE;
	}

	php_stream_close(stream);
}
