static void php_zlib_output_compression_start(TSRMLS_D)
{
	zval *zoh;
	php_output_handler *h;

	switch (ZLIBG(output_compression)) {
		case 0:
			break;
		case 1:
			ZLIBG(output_compression) = PHP_OUTPUT_HANDLER_DEFAULT_SIZE;
			 
		default:
			if (	php_zlib_output_encoding(TSRMLS_C) &&
					(h = php_zlib_output_handler_init(ZEND_STRL(PHP_ZLIB_OUTPUT_HANDLER_NAME), ZLIBG(output_compression), PHP_OUTPUT_HANDLER_STDFLAGS TSRMLS_CC)) &&
					(SUCCESS == php_output_handler_start(h TSRMLS_CC))) {
				if (ZLIBG(output_handler) && *ZLIBG(output_handler)) {
					MAKE_STD_ZVAL(zoh);
					ZVAL_STRING(zoh, ZLIBG(output_handler), 1);
					php_output_start_user(zoh, ZLIBG(output_compression), PHP_OUTPUT_HANDLER_STDFLAGS TSRMLS_CC);
					zval_ptr_dtor(&zoh);
				}
			}
			break;
	}
}
