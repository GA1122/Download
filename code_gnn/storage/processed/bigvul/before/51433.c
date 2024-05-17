void gdImageCopyMergeGray (gdImagePtr dst, gdImagePtr src, int dstX, int dstY, int srcX, int srcY, int w, int h, int pct)
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
			 
			if (gdImageGetTransparent(src) == c) {
				tox++;
				continue;
			}

			 
			if (dst == src && pct == 100) {
				nc = c;
			} else {
				dc = gdImageGetPixel(dst, tox, toy);
				g = (0.29900f * gdImageRed(dst, dc)) + (0.58700f * gdImageGreen(dst, dc)) + (0.11400f * gdImageBlue(dst, dc));

				ncR = (int)(gdImageRed (src, c) * (pct / 100.0f) + g * ((100 - pct) / 100.0));
				ncG = (int)(gdImageGreen (src, c) * (pct / 100.0f) + g * ((100 - pct) / 100.0));
				ncB = (int)(gdImageBlue (src, c) * (pct / 100.0f) + g * ((100 - pct) / 100.0));


				 
				nc = gdImageColorExact(dst, ncR, ncG, ncB);
				if (nc == (-1)) {
					 
					nc = gdImageColorAllocate(dst, ncR, ncG, ncB);
					 
					if (nc == (-1)) {
						nc = gdImageColorClosest(dst, ncR, ncG, ncB);
					}
				}
			}
			gdImageSetPixel(dst, tox, toy, nc);
			tox++;
		}
		toy++;
	}
}
