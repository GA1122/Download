PHP_MINIT_FUNCTION(phar)  
{
	REGISTER_INI_ENTRIES();

	phar_orig_compile_file = zend_compile_file;
	zend_compile_file = phar_compile_file;

	phar_save_resolve_path = zend_resolve_path;
	zend_resolve_path = phar_resolve_path;

	phar_object_init();

	phar_intercept_functions_init();
	phar_save_orig_functions();

	return php_register_url_stream_wrapper("phar", &php_stream_phar_wrapper);
}
 
