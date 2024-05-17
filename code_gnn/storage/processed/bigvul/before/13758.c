static void clean_module_classes(int module_number TSRMLS_DC)  
{
	zend_hash_apply_with_argument(EG(class_table), (apply_func_arg_t) clean_module_class, (void *) &module_number TSRMLS_CC);
}
 
