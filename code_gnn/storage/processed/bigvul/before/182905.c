  void gdImageGifCtx(gdImagePtr im, gdIOCtxPtr out)
// {
// 	_gdImageGifCtx(im, out);
// }
// 
//  
// static int _gdImageGifCtx(gdImagePtr im, gdIOCtxPtr out)
  {
  	gdImagePtr pim = 0, tim = im;
  	int interlace, BitsPerPixel;
 	interlace = im->interlace;
 	if (im->trueColor) {
 		 
  		pim = gdImageCreatePaletteFromTrueColor(im, 1, 256);
  		if (!pim) {
			return;
// 			return 1;
  		}
  		tim = pim;
  	}
 	BitsPerPixel = colorstobpp(tim->colorsTotal);
 	 
 	GIFEncode(
 		out, tim->sx, tim->sy, tim->interlace, 0, tim->transparent, BitsPerPixel,
 		tim->red, tim->green, tim->blue, tim);
 	if (pim) {
  		 
  		gdImageDestroy(	pim);
  	}
// 
//     return 0;
  }