static void ensure_incremental_xref(fz_context *ctx, pdf_document *doc)
{
	 
	if ((doc->num_incremental_sections == 0 || doc->xref_sections[0].unsaved_sigs != NULL)
		&& !doc->disallow_new_increments)
	{
		pdf_xref *xref = &doc->xref_sections[0];
		pdf_xref *pxref;
		pdf_xref_entry *new_table = fz_calloc(ctx, xref->num_objects, sizeof(pdf_xref_entry));
		pdf_xref_subsec *sub;
		pdf_obj *trailer = NULL;
		int i;

		fz_var(trailer);
		fz_try(ctx)
		{
			sub = fz_malloc_struct(ctx, pdf_xref_subsec);
			trailer = xref->trailer ? pdf_copy_dict(ctx, xref->trailer) : NULL;
			doc->xref_sections = fz_resize_array(ctx, doc->xref_sections, doc->num_xref_sections + 1, sizeof(pdf_xref));
			xref = &doc->xref_sections[0];
			pxref = &doc->xref_sections[1];
			memmove(pxref, xref, doc->num_xref_sections * sizeof(pdf_xref));
			 
			xref->subsec = sub;
			xref->trailer = trailer;
			xref->pre_repair_trailer = NULL;
			xref->unsaved_sigs = NULL;
			xref->unsaved_sigs_end = NULL;
			sub->next = NULL;
			sub->len = xref->num_objects;
			sub->start = 0;
			sub->table = new_table;
			doc->num_xref_sections++;
			doc->num_incremental_sections++;
		}
		fz_catch(ctx)
		{
			fz_free(ctx, new_table);
			pdf_drop_obj(ctx, trailer);
			fz_rethrow(ctx);
		}

		 
		for (i = 0; i < doc->max_xref_len; i++)
		{
			doc->xref_index[i]++;
		}
	}
}
