PHP_FUNCTION(imagefill)
{
	zval *IM;
	long x, y, col;
	gdImagePtr im;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rlll", &IM, &x, &y, &col) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(im, gdImagePtr, &IM, -1, "Image", le_gd);
	gdImageFill(im, x, y, col);
	RETURN_TRUE;
}
