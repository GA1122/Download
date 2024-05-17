pdf_array_get(fz_context *ctx, pdf_obj *obj, int i)
{
	RESOLVE(obj);
	if (!OBJ_IS_ARRAY(obj))
		return NULL;
	if (i < 0 || i >= ARRAY(obj)->len)
		return NULL;
	return ARRAY(obj)->items[i];
}
