PHP_FUNCTION(imagecolormatch)
{
	zval *IM1, *IM2;
	gdImagePtr im1, im2;
	int result;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rr", &IM1, &IM2) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(im1, gdImagePtr, &IM1, -1, "Image", le_gd);
	ZEND_FETCH_RESOURCE(im2, gdImagePtr, &IM2, -1, "Image", le_gd);

	result = gdImageColorMatch(im1, im2);
	switch (result) {
		case -1:
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Image1 must be TrueColor" );
			RETURN_FALSE;
			break;
		case -2:
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Image2 must be Palette" );
			RETURN_FALSE;
			break;
		case -3:
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Image1 and Image2 must be the same size" );
			RETURN_FALSE;
			break;
		case -4:
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Image2 must have at least one color" );
			RETURN_FALSE;
			break;
	}

	RETURN_TRUE;
}
