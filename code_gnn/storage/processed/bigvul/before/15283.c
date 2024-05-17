static void apply_filter_to_stream(int append, INTERNAL_FUNCTION_PARAMETERS)
{
	zval *zstream;
	php_stream *stream;
	char *filtername;
	int filternamelen;
	long read_write = 0;
	zval *filterparams = NULL;
	php_stream_filter *filter = NULL;
	int ret;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs|lz", &zstream,
				&filtername, &filternamelen, &read_write, &filterparams) == FAILURE) {
		RETURN_FALSE;
	}

	php_stream_from_zval(stream, &zstream);

	if ((read_write & PHP_STREAM_FILTER_ALL) == 0) {
		 
		if (strchr(stream->mode, 'r') || strchr(stream->mode, '+')) {
			read_write |= PHP_STREAM_FILTER_READ;
		}
		if (strchr(stream->mode, 'w') || strchr(stream->mode, '+') || strchr(stream->mode, 'a')) {
			read_write |= PHP_STREAM_FILTER_WRITE;
		}
	}

	if (read_write & PHP_STREAM_FILTER_READ) {
		filter = php_stream_filter_create(filtername, filterparams, php_stream_is_persistent(stream) TSRMLS_CC);
		if (filter == NULL) {
			RETURN_FALSE;
		}

		if (append) {
			ret = php_stream_filter_append_ex(&stream->readfilters, filter TSRMLS_CC);
		} else {
			ret = php_stream_filter_prepend_ex(&stream->readfilters, filter TSRMLS_CC);
		}
		if (ret != SUCCESS) {
			php_stream_filter_remove(filter, 1 TSRMLS_CC);
			RETURN_FALSE;
		}
	}

	if (read_write & PHP_STREAM_FILTER_WRITE) {
		filter = php_stream_filter_create(filtername, filterparams, php_stream_is_persistent(stream) TSRMLS_CC);
		if (filter == NULL) {
			RETURN_FALSE;
		}

		if (append) {
			ret = php_stream_filter_append_ex(&stream->writefilters, filter TSRMLS_CC);
		} else {
			ret = php_stream_filter_prepend_ex(&stream->writefilters, filter TSRMLS_CC);
		}
		if (ret != SUCCESS) {
			php_stream_filter_remove(filter, 1 TSRMLS_CC);
			RETURN_FALSE;
		}
	}

	if (filter) {
		RETURN_RESOURCE(filter->rsrc_id = ZEND_REGISTER_RESOURCE(NULL, filter, php_file_le_stream_filter()));
	} else {
		RETURN_FALSE;
	}
}
