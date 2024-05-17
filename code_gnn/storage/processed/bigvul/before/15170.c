PHP_FUNCTION(imagepsextendfont)
{
	zval *fnt;
	double ext;
	int *f_ind;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rd", &fnt, &ext) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(f_ind, int *, &fnt, -1, "Type 1 font", le_ps_font);

	T1_DeleteAllSizes(*f_ind);

	if (ext <= 0) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Second parameter %F out of range (must be > 0)", ext);
		RETURN_FALSE;
	}

	if (T1_ExtendFont(*f_ind, ext) != 0) {
		RETURN_FALSE;
	}

	RETURN_TRUE;
}
