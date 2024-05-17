ZEND_API void zend_timeout(int dummy)  
{

	if (zend_on_timeout) {
#ifdef ZEND_SIGNALS
		 
		SIGG(running) = 0;
#endif
		zend_on_timeout(EG(timeout_seconds));
	}

	zend_error_noreturn(E_ERROR, "Maximum execution time of %pd second%s exceeded", EG(timeout_seconds), EG(timeout_seconds) == 1 ? "" : "s");
}
 
