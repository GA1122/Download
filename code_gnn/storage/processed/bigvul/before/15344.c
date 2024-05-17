static PHP_FUNCTION(gzfile)
{
	char *filename;
	int filename_len;
	int flags = REPORT_ERRORS;
	char buf[8192] = {0};
	register int i = 0;
	long use_include_path = 0;
	php_stream *stream;

	if (SUCCESS != zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "p|l", &filename, &filename_len, &use_include_path)) {
		return;
	}

	if (use_include_path) {
		flags |= USE_PATH;
	}

	 
	stream = php_stream_gzopen(NULL, filename, "rb", flags, NULL, NULL STREAMS_CC TSRMLS_CC);

	if (!stream) {
		 
		RETURN_FALSE;
	}

	 
	array_init(return_value);

	 
	memset(buf, 0, sizeof(buf));
	    
	while (php_stream_gets(stream, buf, sizeof(buf) - 1) != NULL) {
		add_index_string(return_value, i++, buf, 1);
	}
	php_stream_close(stream);
}
