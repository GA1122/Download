PHP_FUNCTION(imagepsslantfont)
{
	zval *fnt;
	double slt;
	int *f_ind;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rd", &fnt, &slt) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(f_ind, int *, &fnt, -1, "Type 1 font", le_ps_font);

	if (T1_SlantFont(*f_ind, slt) != 0) {
		RETURN_FALSE;
	}

	RETURN_TRUE;
}
