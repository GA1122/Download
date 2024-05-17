BGD_DECLARE(void) gdImageCopyMergeGray (gdImagePtr dst, gdImagePtr src, int dstX, int dstY,
										int srcX, int srcY, int w, int h, int pct)
{

	int c, dc;
	int x, y;
	int tox, toy;
	int ncR, ncG, ncB;
	float g;
	toy = dstY;
	for (y = srcY; (y < (srcY + h)); y++) {
		tox = dstX;
		for (x = srcX; (x < (srcX + w)); x++) {
			int nc;
			c = gdImageGetPixel (src, x, y);
			 
			if (gdImageGetTransparent (src) == c) {
				tox++;
				continue;
			}
			 
			if (dst == src && pct == 100) {
				nc = c;
			} else {
				dc = gdImageGetPixel (dst, tox, toy);
				g = 0.29900 * gdImageRed(dst, dc)
				    + 0.58700 * gdImageGreen(dst, dc) + 0.11400 * gdImageBlue(dst, dc);

				ncR = gdImageRed (src, c) * (pct / 100.0)
				      + g * ((100 - pct) / 100.0);
				ncG = gdImageGreen (src, c) * (pct / 100.0)
				      + g * ((100 - pct) / 100.0);
				ncB = gdImageBlue (src, c) * (pct / 100.0)
				      + g * ((100 - pct) / 100.0);

				 
				nc = gdImageColorExact (dst, ncR, ncG, ncB);
				if (nc == (-1)) {
					 
					nc = gdImageColorAllocate (dst, ncR, ncG, ncB);
					 
					if (nc == (-1)) {
						nc = gdImageColorClosest (dst, ncR, ncG, ncB);
					}
				}
			}
			gdImageSetPixel (dst, tox, toy, nc);
			tox++;
		}
		toy++;
	}
}
