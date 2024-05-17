PHPAPI PHP_FUNCTION(fgetc)
{
	zval *arg1;
	char buf[2];
	int result;
	php_stream *stream;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &arg1) == FAILURE) {
		RETURN_FALSE;
	}

	PHP_STREAM_TO_ZVAL(stream, &arg1);

	result = php_stream_getc(stream);

	if (result == EOF) {
		RETVAL_FALSE;
	} else {
		buf[0] = result;
		buf[1] = '\0';

		RETURN_STRINGL(buf, 1, 1);
	}
}
