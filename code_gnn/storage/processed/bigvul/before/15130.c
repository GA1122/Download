PHP_FUNCTION(imagepalettecopy)
{
	zval *dstim, *srcim;
	gdImagePtr dst, src;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rr", &dstim, &srcim) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(dst, gdImagePtr, &dstim, -1, "Image", le_gd);
	ZEND_FETCH_RESOURCE(src, gdImagePtr, &srcim, -1, "Image", le_gd);

	gdImagePaletteCopy(dst, src);
}
