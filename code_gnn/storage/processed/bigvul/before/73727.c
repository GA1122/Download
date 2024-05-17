BGD_DECLARE(void) gdImageTiff(gdImagePtr im, FILE *outFile)
{
	gdIOCtx *out = gdNewFileCtx(outFile);
	if (out == NULL) return;
	gdImageTiffCtx(im, out);  
	out->gd_free(out);
}
