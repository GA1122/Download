PHP_FUNCTION(finfo_set_flags)
{
	long options;
	struct php_fileinfo *finfo;
	zval *zfinfo;
	FILEINFO_DECLARE_INIT_OBJECT(object)

	if (object) {
		if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &options) == FAILURE) {
			RETURN_FALSE;
		}
		FILEINFO_FROM_OBJECT(finfo, object);
	} else {
		if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rl", &zfinfo, &options) == FAILURE) {
			RETURN_FALSE;
		}
		ZEND_FETCH_RESOURCE(finfo, struct php_fileinfo *, &zfinfo, -1, "file_info", le_fileinfo);
	}

	FINFO_SET_OPTION(finfo->magic, options)
	finfo->options = options;

	RETURN_TRUE;
}
