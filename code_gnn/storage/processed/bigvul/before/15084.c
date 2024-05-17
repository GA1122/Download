PHP_FUNCTION(finfo_close)
{
	struct php_fileinfo *finfo;
	zval *zfinfo;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zfinfo) == FAILURE) {
		RETURN_FALSE;
	}
	ZEND_FETCH_RESOURCE(finfo, struct php_fileinfo *, &zfinfo, -1, "file_info", le_fileinfo);

	zend_list_delete(Z_RESVAL_P(zfinfo));

	RETURN_TRUE;
}
