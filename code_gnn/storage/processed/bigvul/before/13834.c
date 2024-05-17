ZEND_API void zend_restore_error_handling(zend_error_handling *saved TSRMLS_DC)  
{
	EG(error_handling) = saved->handling;
	EG(exception_class) = saved->handling == EH_THROW ? saved->exception : NULL;
	if (saved->user_handler	&& saved->user_handler != EG(user_error_handler)) {
		if (EG(user_error_handler)) {
			zval_ptr_dtor(&EG(user_error_handler));
		}
		EG(user_error_handler) = saved->user_handler;
	} else if (saved->user_handler) {
		zval_ptr_dtor(&saved->user_handler);
	}
	saved->user_handler = NULL;
}
 
