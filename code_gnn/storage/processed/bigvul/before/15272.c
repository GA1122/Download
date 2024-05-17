PHP_FUNCTION(stream_filter_remove)
{
	zval *zfilter;
	php_stream_filter *filter;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zfilter) == FAILURE) {
		RETURN_FALSE;
	}

	filter = zend_fetch_resource(&zfilter TSRMLS_CC, -1, NULL, NULL, 1, php_file_le_stream_filter());
	if (!filter) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid resource given, not a stream filter");
		RETURN_FALSE;
	}

	if (php_stream_filter_flush(filter, 1) == FAILURE) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unable to flush filter, not removing");
		RETURN_FALSE;
	}

	if (zend_list_delete(Z_LVAL_P(zfilter)) == FAILURE) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Could not invalidate filter, not removing");
		RETURN_FALSE;
	} else {
		php_stream_filter_remove(filter, 1 TSRMLS_CC);
		RETURN_TRUE;
	}
}
