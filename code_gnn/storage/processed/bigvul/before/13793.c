ZEND_API void zend_fcall_info_args_restore(zend_fcall_info *fci, int param_count, zval ***params)  
{
	zend_fcall_info_args_clear(fci, 1);
	fci->param_count = param_count;
	fci->params = params;
}
 
