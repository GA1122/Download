int module_registry_unload_temp(const zend_module_entry *module TSRMLS_DC)  
{
	return (module->type == MODULE_TEMPORARY) ? ZEND_HASH_APPLY_REMOVE : ZEND_HASH_APPLY_STOP;
}
 
