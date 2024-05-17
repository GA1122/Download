PHP_MSHUTDOWN_FUNCTION(phar)  
{
	php_unregister_url_stream_wrapper("phar" TSRMLS_CC);

	phar_intercept_functions_shutdown(TSRMLS_C);

	if (zend_compile_file == phar_compile_file) {
		zend_compile_file = phar_orig_compile_file;
	}

	if (PHAR_G(manifest_cached)) {
		zend_hash_destroy(&(cached_phars));
		zend_hash_destroy(&(cached_alias));
	}

	return SUCCESS;
}
 