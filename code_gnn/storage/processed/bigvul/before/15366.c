static php_output_handler *php_zlib_output_handler_init(const char *handler_name, size_t handler_name_len, size_t chunk_size, int flags TSRMLS_DC)
{
	php_output_handler *h = NULL;

	if (!ZLIBG(output_compression)) {
		ZLIBG(output_compression) = chunk_size ? chunk_size : PHP_OUTPUT_HANDLER_DEFAULT_SIZE;
	}

    ZLIBG(handler_registered) = 1;

	if ((h = php_output_handler_create_internal(handler_name, handler_name_len, php_zlib_output_handler, chunk_size, flags TSRMLS_CC))) {
		php_output_handler_set_context(h, php_zlib_output_handler_context_init(TSRMLS_C), php_zlib_output_handler_context_dtor TSRMLS_CC);
	}

	return h;
}
