ZEND_API zend_bool zend_is_callable(zval *callable, uint check_flags, char **callable_name TSRMLS_DC)  
{
	return zend_is_callable_ex(callable, NULL, check_flags, callable_name, NULL, NULL, NULL TSRMLS_CC);
}
 
