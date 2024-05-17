pdf_array_push(fz_context *ctx, pdf_obj *obj, pdf_obj *item)
{
	RESOLVE(obj);
	if (!OBJ_IS_ARRAY(obj))
		fz_throw(ctx, FZ_ERROR_GENERIC, "not an array (%s)", pdf_objkindstr(obj));

	if (!item)
		item = PDF_OBJ_NULL;

	prepare_object_for_alteration(ctx, obj, item);
	if (ARRAY(obj)->len + 1 > ARRAY(obj)->cap)
		pdf_array_grow(ctx, ARRAY(obj));
	ARRAY(obj)->items[ARRAY(obj)->len] = pdf_keep_obj(ctx, item);
	ARRAY(obj)->len++;
}
