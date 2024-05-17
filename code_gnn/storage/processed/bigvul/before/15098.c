PHP_FUNCTION(imagelayereffect)
{
	zval *IM;
	long effect;
	gdImagePtr im;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rl", &IM, &effect) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(im, gdImagePtr, &IM, -1, "Image", le_gd);
	gdImageAlphaBlending(im, effect);

	RETURN_TRUE;
}
