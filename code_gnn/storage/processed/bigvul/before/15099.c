PHP_FUNCTION(imagecolorallocatealpha)
{
	zval *IM;
	long red, green, blue, alpha;
	gdImagePtr im;
	int ct = (-1);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rllll", &IM, &red, &green, &blue, &alpha) == FAILURE) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE(im, gdImagePtr, &IM, -1, "Image", le_gd);
	ct = gdImageColorAllocateAlpha(im, red, green, blue, alpha);
	if (ct < 0) {
		RETURN_FALSE;
	}
	RETURN_LONG((long)ct);
}
