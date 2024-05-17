pdf_dict_getsa(fz_context *ctx, pdf_obj *obj, const char *key, const char *abbrev)
{
	pdf_obj *v;
	v = pdf_dict_gets(ctx, obj, key);
	if (v)
		return v;
	return pdf_dict_gets(ctx, obj, abbrev);
}
