ZEND_API const char *zend_get_module_version(const char *module_name)  
{
	char *lname;
	int name_len = strlen(module_name);
	zend_module_entry *module;

	lname = zend_str_tolower_dup(module_name, name_len);
	if (zend_hash_find(&module_registry, lname, name_len + 1, (void**)&module) == FAILURE) {
		efree(lname);
		return NULL;
	}
	efree(lname);
	return module->version;
}
 
