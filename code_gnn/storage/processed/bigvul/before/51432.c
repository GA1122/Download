void gdImageCopy (gdImagePtr dst, gdImagePtr src, int dstX, int dstY, int srcX, int srcY, int w, int h)
{
	int c;
	int x, y;
	int tox, toy;
	int i;
	int colorMap[gdMaxColors];

	if (dst->trueColor) {
		 
		 

		if (src->trueColor) {
			for (y = 0; (y < h); y++) {
				for (x = 0; (x < w); x++) {
					int c = gdImageGetTrueColorPixel (src, srcX + x, srcY + y);
					gdImageSetPixel (dst, dstX + x, dstY + y, c);
				}
			}
		} else {
			 
			for (y = 0; (y < h); y++) {
				for (x = 0; (x < w); x++) {
					int c = gdImageGetPixel (src, srcX + x, srcY + y);
					if (c != src->transparent) {
						gdImageSetPixel(dst, dstX + x, dstY + y, gdTrueColorAlpha(src->red[c], src->green[c], src->blue[c], src->alpha[c]));
					}
				}
			}
		}
		return;
	}

	 
	if (src->trueColor) {  
		toy = dstY;
		for (y = srcY; (y < (srcY + h)); y++) {
			tox = dstX;
			for (x = srcX; x < (srcX + w); x++) {
				int nc;
				c = gdImageGetPixel (src, x, y);

				 
				nc = gdImageColorResolveAlpha(dst, gdTrueColorGetRed(c), gdTrueColorGetGreen(c), gdTrueColorGetBlue(c), gdTrueColorGetAlpha(c));

				gdImageSetPixel(dst, tox, toy, nc);
				tox++;
			}
			toy++;
		}
		return;
	}

	 
	for (i = 0; i < gdMaxColors; i++) {
		colorMap[i] = (-1);
	}
	toy = dstY;
	for (y = srcY; y < (srcY + h); y++) {
		tox = dstX;
		for (x = srcX; x < (srcX + w); x++) {
			int nc;
			int mapTo;
			c = gdImageGetPixel (src, x, y);
			 
			if (gdImageGetTransparent (src) == c) {
				tox++;
				continue;
			}
			 
			if (src->trueColor) {
				 
				mapTo = gdImageColorResolveAlpha (dst, gdTrueColorGetRed (c), gdTrueColorGetGreen (c), gdTrueColorGetBlue (c), gdTrueColorGetAlpha (c));
			} else if (colorMap[c] == (-1)) {
				 
				if (dst == src) {
					nc = c;
				} else {
					 
					nc = gdImageColorResolveAlpha (dst, src->red[c], src->green[c], src->blue[c], src->alpha[c]);
				}
				colorMap[c] = nc;
				mapTo = colorMap[c];
			} else {
				mapTo = colorMap[c];
			}
			gdImageSetPixel (dst, tox, toy, mapTo);
			tox++;
		}
		toy++;
	}
}
