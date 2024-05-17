pdf_dict_gets(fz_context *ctx, pdf_obj *obj, const char *key)
{
	int i;

	RESOLVE(obj);
	if (!OBJ_IS_DICT(obj))
		return NULL;
	if (!key)
		return NULL;

	i = pdf_dict_finds(ctx, obj, key);
	if (i >= 0)
		return DICT(obj)->items[i].v;
	return NULL;
}
