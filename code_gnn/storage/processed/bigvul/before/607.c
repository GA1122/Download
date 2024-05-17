pdf_xobject_colorspace(fz_context *ctx, pdf_xobject *xobj)
{
	pdf_obj *group = pdf_dict_get(ctx, xobj->obj, PDF_NAME_Group);
	if (group)
	{
		pdf_obj *cs = pdf_dict_get(ctx, group, PDF_NAME_CS);
		if (cs)
		{
			fz_colorspace *colorspace = NULL;
			fz_try(ctx)
				colorspace = pdf_load_colorspace(ctx, cs);
			fz_catch(ctx)
				fz_warn(ctx, "cannot load xobject colorspace");
			return colorspace;
		}
	}
	return NULL;
}
