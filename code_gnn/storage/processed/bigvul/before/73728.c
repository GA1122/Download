BGD_DECLARE(void) gdImageTiffCtx(gdImagePtr image, gdIOCtx *out)
{
	int clipx1P, clipy1P, clipx2P, clipy2P;
	int bitDepth = 24;

	 
	gdImageGetClip(image, &clipx1P, &clipy1P, &clipx2P, &clipy2P);

	 
	if(image->trueColor) {
		bitDepth = 24;
	} else if(image->colorsTotal == 2) {
		bitDepth = 1;
	} else {
		bitDepth = 8;
	}

	tiffWriter(image, out, bitDepth);

	 
	gdImageSetClip(image, clipx1P, clipy1P, clipx2P, clipy2P);
}
