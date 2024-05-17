static PHP_MSHUTDOWN_FUNCTION(zlib)
{
	php_unregister_url_stream_wrapper("zlib" TSRMLS_CC);
	php_stream_filter_unregister_factory("zlib.*" TSRMLS_CC);

	UNREGISTER_INI_ENTRIES();

	return SUCCESS;
}
