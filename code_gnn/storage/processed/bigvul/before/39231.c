PHP_FUNCTION(imagescale)
{
	zval *IM;
	gdImagePtr im;
	gdImagePtr im_scaled;
	int new_width, new_height = -1;
	gdInterpolationMethod method = GD_BILINEAR_FIXED;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rl|ll", &IM, &new_width, &new_height, &method) == FAILURE)  {
		return;
	}

	ZEND_FETCH_RESOURCE(im, gdImagePtr, &IM, -1, "Image", le_gd);
	im_scaled = gdImageScale(im, new_width, new_height);
	goto finish;
	switch (method) {
		case GD_NEAREST_NEIGHBOUR:
			im_scaled = gdImageScaleNearestNeighbour(im, new_width, new_height);
			break;

		case GD_BILINEAR_FIXED:
			im_scaled = gdImageScaleBilinear(im, new_width, new_height);
			break;

		case GD_BICUBIC:
			im_scaled = gdImageScaleBicubicFixed(im, new_width, new_height);
			break;

		case GD_BICUBIC_FIXED:
			im_scaled = gdImageScaleBicubicFixed(im, new_width, new_height);
			break;

		default:
			im_scaled = gdImageScaleTwoPass(im, im->sx, im->sy, new_width, new_height);
			break;

	}
finish:
	if (im_scaled == NULL) {
		RETURN_FALSE;
	} else {
		ZEND_REGISTER_RESOURCE(return_value, im_scaled, le_gd);
	}
}
