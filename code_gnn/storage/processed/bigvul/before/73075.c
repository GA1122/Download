BGD_DECLARE(void) gdImagePaletteCopy (gdImagePtr to, gdImagePtr from)
{
	int i;
	int x, y, p;
	int xlate[256];
	if (to->trueColor) {
		return;
	}
	if (from->trueColor) {
		return;
	}

	for (i = 0; i < 256; i++) {
		xlate[i] = -1;
	};

	for (y = 0; y < (to->sy); y++) {
		for (x = 0; x < (to->sx); x++) {
			 
			p = to->pixels[y][x];
			if (xlate[p] == -1) {
				 
				xlate[p] =
				    gdImageColorClosestAlpha (from, to->red[p], to->green[p],
				                              to->blue[p], to->alpha[p]);
				 
				 
				 
			};
			 
			to->pixels[y][x] = xlate[p];
		};
	};

	for (i = 0; (i < (from->colorsTotal)); i++) {
		 
		to->red[i] = from->red[i];
		to->blue[i] = from->blue[i];
		to->green[i] = from->green[i];
		to->alpha[i] = from->alpha[i];
		to->open[i] = 0;
	};

	for (i = from->colorsTotal; (i < to->colorsTotal); i++) {
		to->open[i] = 1;
	};

	to->colorsTotal = from->colorsTotal;

}
