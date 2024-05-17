ZEND_API int zend_fcall_info_args(zend_fcall_info *fci, zval *args TSRMLS_DC)  
{
	HashPosition pos;
	zval **arg, ***params;

	zend_fcall_info_args_clear(fci, !args);

	if (!args) {
		return SUCCESS;
	}

	if (Z_TYPE_P(args) != IS_ARRAY) {
		return FAILURE;
	}

	fci->param_count = zend_hash_num_elements(Z_ARRVAL_P(args));
	fci->params = params = (zval ***) erealloc(fci->params, fci->param_count * sizeof(zval **));

	zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(args), &pos);
	while (zend_hash_get_current_data_ex(Z_ARRVAL_P(args), (void *) &arg, &pos) == SUCCESS) {
		*params++ = arg;
		zend_hash_move_forward_ex(Z_ARRVAL_P(args), &pos);
	}

	return SUCCESS;
}
 
