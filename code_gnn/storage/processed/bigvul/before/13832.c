ZEND_API void zend_replace_error_handling(zend_error_handling_t error_handling, zend_class_entry *exception_class, zend_error_handling *current TSRMLS_DC)  
{
	if (current) {
		zend_save_error_handling(current TSRMLS_CC);
		if (error_handling != EH_NORMAL && EG(user_error_handler)) {
			zval_ptr_dtor(&EG(user_error_handler));
			EG(user_error_handler) = NULL;
		}
	}
	EG(error_handling) = error_handling;
	EG(exception_class) = error_handling == EH_THROW ? exception_class : NULL;
}
 
