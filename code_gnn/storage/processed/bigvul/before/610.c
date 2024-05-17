pdf_xobject_resources(fz_context *ctx, pdf_xobject *xobj)
{
	return pdf_dict_get(ctx, xobj->obj, PDF_NAME_Resources);
}
