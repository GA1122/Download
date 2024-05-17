PHP_FUNCTION(imagecolorclosestalpha)
{
	zval *IM;
	long red, green, blue, alpha;
	gdImagePtr im;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rllll", &IM, &red, &green, &blue, &alpha) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(im, gdImagePtr, &IM, -1, "Image", le_gd);

	RETURN_LONG(gdImageColorClosestAlpha(im, red, green, blue, alpha));
}
