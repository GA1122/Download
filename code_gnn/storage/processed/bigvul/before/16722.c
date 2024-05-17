pdf_resolve_indirect(fz_context *ctx, pdf_obj *ref)
{
	if (pdf_is_indirect(ctx, ref))
	{
		pdf_document *doc = pdf_get_indirect_document(ctx, ref);
		int num = pdf_to_num(ctx, ref);
		pdf_xref_entry *entry;

		if (!doc)
			return NULL;
		if (num <= 0)
		{
			fz_warn(ctx, "invalid indirect reference (%d 0 R)", num);
			return NULL;
		}

		fz_try(ctx)
			entry = pdf_cache_object(ctx, doc, num);
		fz_catch(ctx)
		{
			fz_rethrow_if(ctx, FZ_ERROR_TRYLATER);
			fz_warn(ctx, "cannot load object (%d 0 R) into cache", num);
			return NULL;
		}

		ref = entry->obj;
	}
	return ref;
}
