ZEND_API int zend_fcall_info_init(zval *callable, uint check_flags, zend_fcall_info *fci, zend_fcall_info_cache *fcc, char **callable_name, char **error TSRMLS_DC)  
{
	if (!zend_is_callable_ex(callable, NULL, check_flags, callable_name, NULL, fcc, error TSRMLS_CC)) {
		return FAILURE;
	}

	fci->size = sizeof(*fci);
	fci->function_table = fcc->calling_scope ? &fcc->calling_scope->function_table : EG(function_table);
	fci->object_ptr = fcc->object_ptr;
	fci->function_name = callable;
	fci->retval_ptr_ptr = NULL;
	fci->param_count = 0;
	fci->params = NULL;
	fci->no_separation = 1;
	fci->symbol_table = NULL;

	return SUCCESS;
}
 
