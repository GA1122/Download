ZEND_API int zend_copy_parameters_array(int param_count, zval *argument_array TSRMLS_DC)  
{
	void **p;
	int arg_count;

	p = zend_vm_stack_top(TSRMLS_C) - 1;
	arg_count = (int)(zend_uintptr_t) *p;

	if (param_count>arg_count) {
		return FAILURE;
	}

	while (param_count-->0) {
		zval **param = (zval **) p-(arg_count--);
		zval_add_ref(param);
		add_next_index_zval(argument_array, *param);
	}

	return SUCCESS;
}
 
