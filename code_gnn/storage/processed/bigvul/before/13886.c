pdf_dict_get_key(fz_context *ctx, pdf_obj *obj, int i)
{
	RESOLVE(obj);
	if (!OBJ_IS_DICT(obj))
		return NULL;
	if (i < 0 || i >= DICT(obj)->len)
		return NULL;
	return DICT(obj)->items[i].k;
}
