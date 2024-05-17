static int clean_module_class(const zend_class_entry **ce, int *module_number TSRMLS_DC)  
{
	if ((*ce)->type == ZEND_INTERNAL_CLASS && (*ce)->info.internal.module->module_number == *module_number) {
		return ZEND_HASH_APPLY_REMOVE;
	} else {
		return ZEND_HASH_APPLY_KEEP;
	}
}
 
