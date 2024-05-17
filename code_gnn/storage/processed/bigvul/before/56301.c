BGD_DECLARE(gdImagePtr) gdImageRotateInterpolated(const gdImagePtr src, const float angle, int bgcolor)
{
	 
	const int angle_rounded = fmod((int) floorf(angle * 100), 360 * 100);

	if (bgcolor < 0) {
		return NULL;
	}

	 
	switch (angle_rounded) {
		case    0: {
			gdImagePtr dst = gdImageClone(src);

			if (dst == NULL) {
				return NULL;
			}
			if (dst->trueColor == 0) {
				gdImagePaletteToTrueColor(dst);
			}
			return dst;
		}

		case -27000:
		case   9000:
			return gdImageRotate90(src, 0);

		case -18000:
		case  18000:
			return gdImageRotate180(src, 0);

		case  -9000:
		case  27000:
			return gdImageRotate270(src, 0);
	}

	if (src == NULL || src->interpolation_id < 1 || src->interpolation_id > GD_METHOD_COUNT) {
		return NULL;
	}

	switch (src->interpolation_id) {
		case GD_NEAREST_NEIGHBOUR:
			return gdImageRotateNearestNeighbour(src, angle, bgcolor);
			break;

		case GD_BILINEAR_FIXED:
			return gdImageRotateBilinear(src, angle, bgcolor);
			break;

		case GD_BICUBIC_FIXED:
			return gdImageRotateBicubicFixed(src, angle, bgcolor);
			break;

		default:
			return gdImageRotateGeneric(src, angle, bgcolor);
	}
	return NULL;
}
