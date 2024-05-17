PHP_FUNCTION(imagefilledellipse)
{
	zval *IM;
	long cx, cy, w, h, color;
	gdImagePtr im;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rlllll", &IM, &cx, &cy, &w, &h, &color) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(im, gdImagePtr, &IM, -1, "Image", le_gd);

	gdImageFilledEllipse(im, cx, cy, w, h, color);

	RETURN_TRUE;
}
