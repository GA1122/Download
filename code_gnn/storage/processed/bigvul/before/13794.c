ZEND_API void zend_fcall_info_args_save(zend_fcall_info *fci, int *param_count, zval ****params)  
{
	*param_count = fci->param_count;
	*params = fci->params;
	fci->param_count = 0;
	fci->params = NULL;
}
 
