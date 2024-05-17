pdf_array_put_drop(fz_context *ctx, pdf_obj *obj, int i, pdf_obj *item)
{
	pdf_array_put(ctx, obj, i, item);
	pdf_drop_obj(ctx, item);
}
