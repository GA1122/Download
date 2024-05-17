ZEND_API int zend_get_module_started(const char *module_name)  
{
	zend_module_entry *module;

	return (zend_hash_find(&module_registry, module_name, strlen(module_name)+1, (void**)&module) == SUCCESS && module->module_started) ? SUCCESS : FAILURE;
}
 
