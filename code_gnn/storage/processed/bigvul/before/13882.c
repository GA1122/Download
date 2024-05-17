pdf_copy_dict(fz_context *ctx, pdf_obj *obj)
{
	pdf_document *doc;
	pdf_obj *dict;
	int i, n;

	RESOLVE(obj);
	if (!OBJ_IS_DICT(obj))
		fz_throw(ctx, FZ_ERROR_GENERIC, "not a dict (%s)", pdf_objkindstr(obj));

	doc = DICT(obj)->doc;
	n = pdf_dict_len(ctx, obj);
	dict = pdf_new_dict(ctx, doc, n);
	for (i = 0; i < n; i++)
		pdf_dict_put(ctx, dict, pdf_dict_get_key(ctx, obj, i), pdf_dict_get_val(ctx, obj, i));

	return dict;
}
