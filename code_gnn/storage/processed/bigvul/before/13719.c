ZEND_API int add_assoc_function(zval *arg, const char *key, void (*function_ptr)(INTERNAL_FUNCTION_PARAMETERS))  
{
	zend_error(E_WARNING, "add_assoc_function() is no longer supported");
	return FAILURE;
}
 
