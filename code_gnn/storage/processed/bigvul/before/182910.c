 void * gdImageWBMPPtr (gdImagePtr im, int *size, int fg)
  {
  	void *rv;
  	gdIOCtx *out = gdNewDynamicCtx(2048, NULL);
	gdImageWBMPCtx(im, fg, out);
	rv = gdDPExtractData(out, size);
// 	if (!_gdImageWBMPCtx(im, fg, out)) {
// 		rv = gdDPExtractData(out, size);
// 	} else {
// 		rv = NULL;
// 	}
  	out->gd_free(out);
  
  	return rv;
 }