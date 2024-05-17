ZEND_API void zend_fcall_info_args_clear(zend_fcall_info *fci, int free_mem)  
{
	if (fci->params) {
		if (free_mem) {
			efree(fci->params);
			fci->params = NULL;
		}
	}
	fci->param_count = 0;
}
 
