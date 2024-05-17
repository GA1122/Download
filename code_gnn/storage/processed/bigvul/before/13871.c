pdf_array_delete(fz_context *ctx, pdf_obj *obj, int i)
{
	RESOLVE(obj);
	if (!OBJ_IS_ARRAY(obj))
		fz_throw(ctx, FZ_ERROR_GENERIC, "not an array (%s)", pdf_objkindstr(obj));
	if (i < 0 || i >= ARRAY(obj)->len)
		fz_throw(ctx, FZ_ERROR_GENERIC, "index out of bounds");

	pdf_drop_obj(ctx, ARRAY(obj)->items[i]);
	ARRAY(obj)->items[i] = 0;
	ARRAY(obj)->len--;
	memmove(ARRAY(obj)->items + i, ARRAY(obj)->items + i + 1, (ARRAY(obj)->len - i) * sizeof(pdf_obj*));
}
