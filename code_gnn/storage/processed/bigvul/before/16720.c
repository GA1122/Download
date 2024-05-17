void pdf_replace_xref(fz_context *ctx, pdf_document *doc, pdf_xref_entry *entries, int n)
{
	pdf_xref *xref = NULL;
	pdf_xref_subsec *sub;
	pdf_obj *trailer = pdf_keep_obj(ctx, pdf_trailer(ctx, doc));

	fz_var(xref);
	fz_try(ctx)
	{
		fz_free(ctx, doc->xref_index);
		doc->xref_index = NULL;  
		doc->xref_index = fz_calloc(ctx, n, sizeof(int));
		xref = fz_malloc_struct(ctx, pdf_xref);
		sub = fz_malloc_struct(ctx, pdf_xref_subsec);

		 
		pdf_drop_xref_sections(ctx, doc);

		sub->table = entries;
		sub->start = 0;
		sub->len = n;
		xref->subsec = sub;
		xref->num_objects = n;
		xref->trailer = trailer;
		trailer = NULL;

		doc->xref_sections = xref;
		doc->num_xref_sections = 1;
		doc->num_incremental_sections = 0;
		doc->xref_base = 0;
		doc->disallow_new_increments = 0;
		doc->max_xref_len = n;

		memset(doc->xref_index, 0, sizeof(int)*doc->max_xref_len);
	}
	fz_catch(ctx)
	{
		fz_free(ctx, xref);
		pdf_drop_obj(ctx, trailer);
		fz_rethrow(ctx);
	}
}
