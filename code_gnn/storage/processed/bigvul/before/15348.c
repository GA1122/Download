static PHP_MINIT_FUNCTION(zlib)
{
	php_register_url_stream_wrapper("compress.zlib", &php_stream_gzip_wrapper TSRMLS_CC);
	php_stream_filter_register_factory("zlib.*", &php_zlib_filter_factory TSRMLS_CC);

	php_output_handler_alias_register(ZEND_STRL("ob_gzhandler"), php_zlib_output_handler_init TSRMLS_CC);
	php_output_handler_conflict_register(ZEND_STRL("ob_gzhandler"), php_zlib_output_conflict_check TSRMLS_CC);
	php_output_handler_conflict_register(ZEND_STRL(PHP_ZLIB_OUTPUT_HANDLER_NAME), php_zlib_output_conflict_check TSRMLS_CC);

	REGISTER_LONG_CONSTANT("FORCE_GZIP", PHP_ZLIB_ENCODING_GZIP, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("FORCE_DEFLATE", PHP_ZLIB_ENCODING_DEFLATE, CONST_CS|CONST_PERSISTENT);

	REGISTER_LONG_CONSTANT("ZLIB_ENCODING_RAW", PHP_ZLIB_ENCODING_RAW, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("ZLIB_ENCODING_GZIP", PHP_ZLIB_ENCODING_GZIP, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("ZLIB_ENCODING_DEFLATE", PHP_ZLIB_ENCODING_DEFLATE, CONST_CS|CONST_PERSISTENT);
	REGISTER_INI_ENTRIES();
	return SUCCESS;
}
