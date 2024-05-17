ZEND_API void zend_save_error_handling(zend_error_handling *current TSRMLS_DC)  
{
	current->handling = EG(error_handling);
	current->exception = EG(exception_class);
	current->user_handler = EG(user_error_handler);
	if (current->user_handler) {
		Z_ADDREF_P(current->user_handler);
	}
}
 
