PHP_FUNCTION(imagesetthickness)
{
	zval *IM;
	long thick;
	gdImagePtr im;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rl", &IM, &thick) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(im, gdImagePtr, &IM, -1, "Image", le_gd);

	gdImageSetThickness(im, thick);

	RETURN_TRUE;
}
