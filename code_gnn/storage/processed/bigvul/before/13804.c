ZEND_API int zend_get_parameters_ex(int param_count, ...)  
{
	void **p;
	int arg_count;
	va_list ptr;
	zval ***param;
	TSRMLS_FETCH();

	p = zend_vm_stack_top(TSRMLS_C) - 1;
	arg_count = (int)(zend_uintptr_t) *p;

	if (param_count>arg_count) {
		return FAILURE;
	}

	va_start(ptr, param_count);
	while (param_count-->0) {
		param = va_arg(ptr, zval ***);
		*param = (zval **) p-(arg_count--);
	}
	va_end(ptr);

	return SUCCESS;
}
 
