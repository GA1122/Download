  void gdImageWBMPCtx (gdImagePtr image, int fg, gdIOCtx * out)
// {
// 	_gdImageWBMPCtx(image, fg, out);
// }
// 
//  
// static int _gdImageWBMPCtx(gdImagePtr image, int fg, gdIOCtx *out)
  {
  	int x, y, pos;
  	Wbmp *wbmp;
  
  	 
  	if ((wbmp = createwbmp (gdImageSX (image), gdImageSY (image), WBMP_WHITE)) == NULL) {
  		gd_error("Could not create WBMP");
		return;
// 		return 1;
  	}
  
  	 
 	pos = 0;
 	for (y = 0; y < gdImageSY(image); y++) {
 		for (x = 0; x < gdImageSX(image); x++) {
 			if (gdImageGetPixel (image, x, y) == fg) {
 				wbmp->bitmap[pos] = WBMP_BLACK;
 			}
 			pos++;
 		}
 	}
  
  	 
  	if (writewbmp (wbmp, &gd_putout, out)) {
// 		freewbmp(wbmp);
  		gd_error("Could not save WBMP");
// 		return 1;
  	}
  	 
  	freewbmp(wbmp);
 }