static void php_session_reset(TSRMLS_D)  
{
	if (PS(session_status) == php_session_active) {
		php_session_initialize(TSRMLS_C);
	}
}
 
