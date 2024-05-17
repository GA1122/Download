PHP_FUNCTION(mcrypt_generic_deinit)
{
	MCRYPT_GET_TD_ARG

	if (mcrypt_generic_deinit(pm->td) < 0) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Could not terminate encryption specifier");
		RETURN_FALSE
	}
	pm->init = 0;
	RETURN_TRUE
}
