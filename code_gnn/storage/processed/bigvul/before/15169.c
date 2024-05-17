PHP_FUNCTION(imagepsfreefont)
{
	zval *fnt;
	int *f_ind;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &fnt) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(f_ind, int *, &fnt, -1, "Type 1 font", le_ps_font);
	zend_list_delete(Z_LVAL_P(fnt));
	RETURN_TRUE;
}
