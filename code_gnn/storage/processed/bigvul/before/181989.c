 BGD_DECLARE(void) gdImageWebpCtx (gdImagePtr im, gdIOCtx * outfile, int quality)
//  
// static int _gdImageWebpCtx (gdImagePtr im, gdIOCtx * outfile, int quality)
  {
  	uint8_t *argb;
  	int x, y;
  	uint8_t *p;
  	uint8_t *out;
  	size_t out_size;
//     int ret = 0;
  
  	if (im == NULL) {
		return;
// 		return 1;
  	}
  
  	if (!gdImageTrueColor(im)) {
		gd_error("Paletter image not supported by webp");
		return;
// 		gd_error("Palette image not supported by webp");
// 		return 1;
  	}
  
  	if (quality == -1) {
  		quality = 80;
  	}
  
  	if (overflow2(gdImageSX(im), 4)) {
		return;
// 		return 1;
  	}
  
  	if (overflow2(gdImageSX(im) * 4, gdImageSY(im))) {
		return;
// 		return 1;
  	}
  
  	argb = (uint8_t *)gdMalloc(gdImageSX(im) * 4 * gdImageSY(im));
  	if (!argb) {
		return;
// 		return 1;
  	}
  	p = argb;
  	for (y = 0; y < gdImageSY(im); y++) {
 		for (x = 0; x < gdImageSX(im); x++) {
 			register int c;
 			register char a;
 			c = im->tpixels[y][x];
 			a = gdTrueColorGetAlpha(c);
 			if (a == 127) {
 				a = 0;
 			} else {
 				a = 255 - ((a << 1) + (a >> 6));
 			}
 			*(p++) = gdTrueColorGetRed(c);
 			*(p++) = gdTrueColorGetGreen(c);
 			*(p++) = gdTrueColorGetBlue(c); 
 			*(p++) = a;
 		}
 	}
  	out_size = WebPEncodeRGBA(argb, gdImageSX(im), gdImageSY(im), gdImageSX(im) * 4, quality, &out);
  	if (out_size == 0) {
  		gd_error("gd-webp encoding failed");
//         ret = 1;
  		goto freeargb;
  	}
  	gdPutBuf(out, out_size, outfile);
  	free(out);
  
  freeargb:
  	gdFree(argb);
// 
//     return ret;
// }
// 
// 
//  
// BGD_DECLARE(void) gdImageWebpCtx (gdImagePtr im, gdIOCtx * outfile, int quality)
// {
// 	_gdImageWebpCtx(im, outfile, quality);
  }