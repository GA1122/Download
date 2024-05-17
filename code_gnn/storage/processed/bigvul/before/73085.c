BGD_DECLARE(void) gdImageSetPixel (gdImagePtr im, int x, int y, int color)
{
	int p;
	switch (color) {
	case gdStyled:
		if (!im->style) {
			 
			return;
		} else {
			p = im->style[im->stylePos++];
		}
		if (p != (gdTransparent)) {
			gdImageSetPixel (im, x, y, p);
		}
		im->stylePos = im->stylePos % im->styleLength;
		break;
	case gdStyledBrushed:
		if (!im->style) {
			 
			return;
		}
		p = im->style[im->stylePos++];
		if ((p != gdTransparent) && (p != 0)) {
			gdImageSetPixel (im, x, y, gdBrushed);
		}
		im->stylePos = im->stylePos % im->styleLength;
		break;
	case gdBrushed:
		gdImageBrushApply (im, x, y);
		break;
	case gdTiled:
		gdImageTileApply (im, x, y);
		break;
	case gdAntiAliased:
		 
		gdImageSetPixel(im, x, y, im->AA_color);
		break;
	default:
		if (gdImageBoundsSafeMacro (im, x, y)) {
			if (im->trueColor) {
				switch (im->alphaBlendingFlag) {
					default:
					case gdEffectReplace:
						im->tpixels[y][x] = color;
						break;
					case gdEffectAlphaBlend:
					case gdEffectNormal:
						im->tpixels[y][x] = gdAlphaBlend(im->tpixels[y][x], color);
						break;
					case gdEffectOverlay :
						im->tpixels[y][x] = gdLayerOverlay(im->tpixels[y][x], color);
						break;
					case gdEffectMultiply :
						im->tpixels[y][x] = gdLayerMultiply(im->tpixels[y][x], color);
						break;
				}
			} else {
				im->pixels[y][x] = color;
			}
		}
		break;
	}
}
