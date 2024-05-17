pdf_load_object(fz_context *ctx, pdf_document *doc, int num)
{
	pdf_xref_entry *entry = pdf_cache_object(ctx, doc, num);
	assert(entry->obj != NULL);
	return pdf_keep_obj(ctx, entry->obj);
}
