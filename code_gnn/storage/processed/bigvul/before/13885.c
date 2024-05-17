pdf_dict_get(fz_context *ctx, pdf_obj *obj, pdf_obj *key)
{
	int i;

	RESOLVE(obj);
	if (!OBJ_IS_DICT(obj))
		return NULL;
	if (!OBJ_IS_NAME(key))
		return NULL;

	if (key < PDF_OBJ_NAME__LIMIT)
		i = pdf_dict_find(ctx, obj, key);
	else
		i = pdf_dict_finds(ctx, obj, pdf_to_name(ctx, key));
	if (i >= 0)
		return DICT(obj)->items[i].v;
	return NULL;
}
