PHP_FUNCTION(fscanf)
{
	int result, format_len, type, argc = 0;
	zval ***args = NULL;
	zval *file_handle;
	char *buf, *format;
	size_t len;
	void *what;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs*", &file_handle, &format, &format_len, &args, &argc) == FAILURE) {
		return;
	}

	what = zend_fetch_resource(&file_handle TSRMLS_CC, -1, "File-Handle", &type, 2, php_file_le_stream(), php_file_le_pstream());

	 
	if (!what) {
		if (args) {
			efree(args);
		}
		RETURN_FALSE;
	}

	buf = php_stream_get_line((php_stream *) what, NULL, 0, &len);
	if (buf == NULL) {
		if (args) {
			efree(args);
		}
		RETURN_FALSE;
	}

	result = php_sscanf_internal(buf, format, argc, args, 0, &return_value TSRMLS_CC);

	if (args) {
		efree(args);
	}
	efree(buf);

	if (SCAN_ERROR_WRONG_PARAM_COUNT == result) {
		WRONG_PARAM_COUNT;
	}
}
