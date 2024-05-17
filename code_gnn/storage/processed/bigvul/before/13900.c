ensure_solid_xref(fz_context *ctx, pdf_document *doc, int num, int which)
{
	pdf_xref *xref = &doc->xref_sections[which];
	pdf_xref_subsec *sub = xref->subsec;
	pdf_xref_subsec *new_sub;

	if (num < xref->num_objects)
		num = xref->num_objects;

	if (sub != NULL && sub->next == NULL && sub->start == 0 && sub->len >= num)
		return;

	new_sub = fz_malloc_struct(ctx, pdf_xref_subsec);
	fz_try(ctx)
	{
		new_sub->table = fz_calloc(ctx, num, sizeof(pdf_xref_entry));
		new_sub->start = 0;
		new_sub->len = num;
		new_sub->next = NULL;
	}
	fz_catch(ctx)
	{
		fz_free(ctx, new_sub);
		fz_rethrow(ctx);
	}

	 
	sub = xref->subsec;
	while (sub != NULL)
	{
		pdf_xref_subsec *next = sub->next;
		int i;

		for (i = 0; i < sub->len; i++)
		{
			new_sub->table[i+sub->start] = sub->table[i];
		}
		fz_free(ctx, sub->table);
		fz_free(ctx, sub);
		sub = next;
	}
	xref->num_objects = num;
	xref->subsec = new_sub;
	if (doc->max_xref_len < num)
		extend_xref_index(ctx, doc, num);
}
