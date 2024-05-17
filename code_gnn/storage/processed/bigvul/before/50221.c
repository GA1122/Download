static void php_session_abort(TSRMLS_D)  
{
	if (PS(session_status) == php_session_active) {
		PS(session_status) = php_session_none;
		if (PS(mod_data) || PS(mod_user_implemented)) {
			PS(mod)->s_close(&PS(mod_data) TSRMLS_CC);
		}
	}
}
 
