pdf_array_len(fz_context *ctx, pdf_obj *obj)
{
	RESOLVE(obj);
	if (!OBJ_IS_ARRAY(obj))
		return 0;
	return ARRAY(obj)->len;
}
