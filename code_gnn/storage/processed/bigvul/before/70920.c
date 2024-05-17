gdImageScaleTwoPass(const gdImagePtr src, const unsigned int new_width,
                    const unsigned int new_height)
{
    const unsigned int src_width = src->sx;
    const unsigned int src_height = src->sy;
	gdImagePtr tmp_im = NULL;
	gdImagePtr dst = NULL;
	int scale_pass_res;

	assert(src != NULL);

     
    if (src_width == new_width && src_height == new_height) {
        return gdImageClone(src);
    } 

	 
	if (!src->trueColor) {
		gdImagePaletteToTrueColor(src);
	} 

     
    if (src_width == new_width) {
        tmp_im = src;
    } else {
        tmp_im = gdImageCreateTrueColor(new_width, src_height);
        if (tmp_im == NULL) {
            return NULL;
        }
        gdImageSetInterpolationMethod(tmp_im, src->interpolation_id);

		scale_pass_res = _gdScalePass(src, src_width, tmp_im, new_width, src_height, HORIZONTAL);
		if (scale_pass_res != 1) {
			gdImageDestroy(tmp_im);
			return NULL;
		}
    } 

     
    if (src_height == new_height) {
        assert(tmp_im != src);
        return tmp_im;
    } 

     
	dst = gdImageCreateTrueColor(new_width, new_height);
	if (dst != NULL) {
        gdImageSetInterpolationMethod(dst, src->interpolation_id);
        scale_pass_res = _gdScalePass(tmp_im, src_height, dst, new_height, new_width, VERTICAL);
		if (scale_pass_res != 1) {
			gdImageDestroy(dst);
			if (src != tmp_im) {
				gdImageDestroy(tmp_im);
			}
			return NULL;
	   }
    } 


	if (src != tmp_im) {
        gdImageDestroy(tmp_im);
    } 

	return dst;
} 