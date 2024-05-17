orphan_object(fz_context *ctx, pdf_document *doc, pdf_obj *obj)
{
	if (doc->orphans_count == doc->orphans_max)
	{
		int new_max = (doc->orphans_max ? doc->orphans_max*2 : 32);

		fz_try(ctx)
		{
			doc->orphans = fz_resize_array(ctx, doc->orphans, new_max, sizeof(*doc->orphans));
			doc->orphans_max = new_max;
		}
		fz_catch(ctx)
		{
			pdf_drop_obj(ctx, obj);
			fz_rethrow(ctx);
		}
	}
	doc->orphans[doc->orphans_count++] = obj;
}
