resolve_properties(fz_context *ctx, pdf_csi *csi, pdf_obj *obj)
{
	if (pdf_is_name(ctx, obj))
		return pdf_dict_get(ctx, pdf_dict_get(ctx, csi->rdb, PDF_NAME_Properties), obj);
	else
		return obj;
}
