ZEND_API int zend_fcall_info_argp(zend_fcall_info *fci TSRMLS_DC, int argc, zval ***argv)  
{
	int i;

	if (argc < 0) {
		return FAILURE;
	}

	zend_fcall_info_args_clear(fci, !argc);

	if (argc) {
		fci->param_count = argc;
		fci->params = (zval ***) erealloc(fci->params, fci->param_count * sizeof(zval **));

		for (i = 0; i < argc; ++i) {
			fci->params[i] = argv[i];
		}
	}

	return SUCCESS;
}
 
