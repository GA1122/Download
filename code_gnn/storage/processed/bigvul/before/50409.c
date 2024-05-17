BGD_DECLARE(gdImagePtr) gdImageCreateFromTgaPtr(int size, void *data)
{
	gdImagePtr im;
	gdIOCtx *in = gdNewDynamicCtxEx (size, data, 0);
	if (in == NULL) return NULL;
	im = gdImageCreateFromTgaCtx(in);
	in->gd_free(in);
	return im;
}