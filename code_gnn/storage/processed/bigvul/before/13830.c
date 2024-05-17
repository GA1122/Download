ZEND_API zend_module_entry* zend_register_internal_module(zend_module_entry *module TSRMLS_DC)  
{
	module->module_number = zend_next_free_module();
	module->type = MODULE_PERSISTENT;
	return zend_register_module_ex(module TSRMLS_CC);
}
 
