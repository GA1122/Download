PHPAPI PHP_FUNCTION(fpassthru)
{
	zval *arg1;
	int size;
	php_stream *stream;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &arg1) == FAILURE) {
		RETURN_FALSE;
	}

	PHP_STREAM_TO_ZVAL(stream, &arg1);

	size = php_stream_passthru(stream);
	RETURN_LONG(size);
}
