static PHP_MSHUTDOWN_FUNCTION(mcrypt)  
{
	php_stream_filter_unregister_factory("mcrypt.*" TSRMLS_CC);
	php_stream_filter_unregister_factory("mdecrypt.*" TSRMLS_CC);

	if (MCG(fd[RANDOM]) > 0) {
		close(MCG(fd[RANDOM]));
	}

	if (MCG(fd[URANDOM]) > 0) {
		close(MCG(fd[URANDOM]));
	}

	UNREGISTER_INI_ENTRIES();
	return SUCCESS;
}
 
