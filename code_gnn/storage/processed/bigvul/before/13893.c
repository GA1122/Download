pdf_dict_len(fz_context *ctx, pdf_obj *obj)
{
	RESOLVE(obj);
	if (!OBJ_IS_DICT(obj))
		return 0;
	return DICT(obj)->len;
}
