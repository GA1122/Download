PHP_FUNCTION(imagefilltoborder)
{
	zval *IM;
	long x, y, border, col;
	gdImagePtr im;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rllll", &IM, &x, &y, &border, &col) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(im, gdImagePtr, &IM, -1, "Image", le_gd);
	gdImageFillToBorder(im, x, y, border, col);
	RETURN_TRUE;
}
