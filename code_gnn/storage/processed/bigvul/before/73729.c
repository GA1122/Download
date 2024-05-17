BGD_DECLARE(void *) gdImageTiffPtr(gdImagePtr im, int *size)
{
	void *rv;
	gdIOCtx *out = gdNewDynamicCtx (2048, NULL);
	if (out == NULL) return NULL;
	gdImageTiffCtx(im, out);  
	rv = gdDPExtractData(out, size);
	out->gd_free(out);
	return rv;
}
