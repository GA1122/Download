ZEND_API int zend_fcall_info_argn(zend_fcall_info *fci TSRMLS_DC, int argc, ...)  
{
	int ret;
	va_list argv;

	va_start(argv, argc);
	ret = zend_fcall_info_argv(fci TSRMLS_CC, argc, &argv);
	va_end(argv);

	return ret;
}
 
