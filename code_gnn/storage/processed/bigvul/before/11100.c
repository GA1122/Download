PHP_MSHUTDOWN_FUNCTION(phar)  
{
	php_unregister_url_stream_wrapper("phar");

	phar_intercept_functions_shutdown();

	if (zend_compile_file == phar_compile_file) {
		zend_compile_file = phar_orig_compile_file;
	}

	if (PHAR_G(manifest_cached)) {
		zend_hash_destroy(&(cached_phars));
		zend_hash_destroy(&(cached_alias));
	}

	return SUCCESS;
}
 
