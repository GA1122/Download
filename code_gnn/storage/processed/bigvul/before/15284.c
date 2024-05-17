static php_stream_context *decode_context_param(zval *contextresource TSRMLS_DC)
{
	php_stream_context *context = NULL;

	context = zend_fetch_resource(&contextresource TSRMLS_CC, -1, NULL, NULL, 1, php_le_stream_context(TSRMLS_C));
	if (context == NULL) {
		php_stream *stream;

		stream = zend_fetch_resource(&contextresource TSRMLS_CC, -1, NULL, NULL, 2, php_file_le_stream(), php_file_le_pstream);

		if (stream) {
			context = stream->context;
			if (context == NULL) {
				 
				context = stream->context = php_stream_context_alloc(TSRMLS_C);
			}
		}
	}

	return context;
}
