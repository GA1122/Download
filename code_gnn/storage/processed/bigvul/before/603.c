pdf_keep_xobject(fz_context *ctx, pdf_xobject *xobj)
{
	return fz_keep_storable(ctx, &xobj->storable);
}
