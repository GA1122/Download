BGD_DECLARE(gdImagePtr) gdImageScale(const gdImagePtr src, const unsigned int new_width, const unsigned int new_height)
{
	gdImagePtr im_scaled = NULL;

	if (src == NULL || (uintmax_t)src->interpolation_id >= GD_METHOD_COUNT) {
		return NULL;
	}

	if (new_width == 0 || new_height == 0) {
		return NULL;
	}
	if (new_width == gdImageSX(src) && new_height == gdImageSY(src)) {
		return gdImageClone(src);
	}
	switch (src->interpolation_id) {
		 
		case GD_NEAREST_NEIGHBOUR:
			im_scaled = gdImageScaleNearestNeighbour(src, new_width, new_height);
			break;

		case GD_BILINEAR_FIXED:
		case GD_LINEAR:
			im_scaled = gdImageScaleBilinear(src, new_width, new_height);
			break;

		case GD_BICUBIC_FIXED:
		case GD_BICUBIC:
			im_scaled = gdImageScaleBicubicFixed(src, new_width, new_height);
			break;

		 
		default:
			if (src->interpolation == NULL) {
				return NULL;
			}
			im_scaled = gdImageScaleTwoPass(src, new_width, new_height);
			break;
	}

	return im_scaled;
}