pdf_array_push_drop(fz_context *ctx, pdf_obj *obj, pdf_obj *item)
{
	RESOLVE(obj);
	if (obj >= PDF_OBJ__LIMIT)
	{
		fz_try(ctx)
			pdf_array_push(ctx, obj, item);
		fz_always(ctx)
			pdf_drop_obj(ctx, item);
		fz_catch(ctx)
			fz_rethrow(ctx);
	}
}
