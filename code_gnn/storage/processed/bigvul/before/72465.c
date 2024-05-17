void gdImageColorTransparent (gdImagePtr im, int color)
{
	if (color < 0) {
		return;
	}
	if (!im->trueColor) {
		if((color >= im->colorsTotal)) {
			return;
		}
		 
		if (im->transparent != -1) {
			im->alpha[im->transparent] = gdAlphaOpaque;
		}
		im->alpha[color] = gdAlphaTransparent;
	}
	im->transparent = color;
}