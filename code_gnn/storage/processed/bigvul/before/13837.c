ZEND_API int zend_startup_module(zend_module_entry *module)  
{
	TSRMLS_FETCH();

	if ((module = zend_register_internal_module(module TSRMLS_CC)) != NULL && zend_startup_module_ex(module TSRMLS_CC) == SUCCESS) {
		return SUCCESS;
	}
	return FAILURE;
}
 
