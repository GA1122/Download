PHPAPI PHP_FUNCTION(fgets)
{
	zval *arg1;
	long len = 1024;
	char *buf = NULL;
	int argc = ZEND_NUM_ARGS();
	size_t line_len = 0;
	php_stream *stream;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r|l", &arg1, &len) == FAILURE) {
		RETURN_FALSE;
	}

	PHP_STREAM_TO_ZVAL(stream, &arg1);

	if (argc == 1) {
		 
		buf = php_stream_get_line(stream, NULL, 0, &line_len);
		if (buf == NULL) {
			goto exit_failed;
		}
	} else if (argc > 1) {
		if (len <= 0) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Length parameter must be greater than 0");
			RETURN_FALSE;
		}

		buf = ecalloc(len + 1, sizeof(char));
		if (php_stream_get_line(stream, buf, len, &line_len) == NULL) {
			goto exit_failed;
		}
	}

	ZVAL_STRINGL(return_value, buf, line_len, 0);
	 
	if (argc > 1 && Z_STRLEN_P(return_value) < len / 2) {
		Z_STRVAL_P(return_value) = erealloc(buf, line_len + 1);
	}
	return;

exit_failed:
	RETVAL_FALSE;
	if (buf) {
		efree(buf);
	}
}
