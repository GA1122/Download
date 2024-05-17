load_font_or_hail_mary(fz_context *ctx, pdf_document *doc, pdf_obj *rdb, pdf_obj *font, int depth, fz_cookie *cookie)
{
	pdf_font_desc *desc;

	fz_try(ctx)
	{
		desc = pdf_load_font(ctx, doc, rdb, font, depth);
	}
	fz_catch(ctx)
	{
		if (fz_caught(ctx) == FZ_ERROR_TRYLATER && cookie && cookie->incomplete_ok)
		{
			desc = NULL;
			cookie->incomplete++;
		}
		else
		{
			fz_rethrow(ctx);
		}
	}
	if (desc == NULL)
		desc = pdf_load_hail_mary_font(ctx, doc);
	return desc;
}
