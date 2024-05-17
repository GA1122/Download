gdImagePtr gdImageCreateFromWBMPCtx (gdIOCtx * infile)
{
	 
	Wbmp *wbmp;
	gdImagePtr im = NULL;
	int black, white;
	int col, row, pos;

	if (readwbmp (&gd_getin, infile, &wbmp)) {
		return NULL;
	}

	if (!(im = gdImageCreate (wbmp->width, wbmp->height))) {
		freewbmp (wbmp);
		return NULL;
	}

	 
	white = gdImageColorAllocate(im, 255, 255, 255);
	 
	black = gdImageColorAllocate(im, 0, 0, 0);

	 
	pos = 0;
	for (row = 0; row < wbmp->height; row++) {
		for (col = 0; col < wbmp->width; col++) {
			if (wbmp->bitmap[pos++] == WBMP_WHITE) {
				gdImageSetPixel(im, col, row, white);
			} else {
				gdImageSetPixel(im, col, row, black);
			}
		}
	}

	freewbmp(wbmp);

	return im;
}
