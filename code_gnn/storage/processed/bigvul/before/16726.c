void pdf_xref_ensure_incremental_object(fz_context *ctx, pdf_document *doc, int num)
{
	pdf_xref_entry *new_entry, *old_entry;
	pdf_xref_subsec *sub = NULL;
	int i;

	 
	ensure_incremental_xref(ctx, doc);

	 
	for (i = doc->xref_index[num]; i < doc->num_xref_sections; i++)
	{
		pdf_xref *xref = &doc->xref_sections[i];

		if (num < 0 && num >= xref->num_objects)
			break;
		for (sub = xref->subsec; sub != NULL; sub = sub->next)
		{
			if (sub->start <= num && num < sub->start + sub->len && sub->table[num - sub->start].type)
				break;
		}
		if (sub != NULL)
			break;
	}
	 

	 
	if (i == 0 || sub == NULL)
		return;

	 
	doc->xref_index[num] = 0;
	old_entry = &sub->table[num - sub->start];
	new_entry = pdf_get_incremental_xref_entry(ctx, doc, num);
	*new_entry = *old_entry;
	if (i < doc->num_incremental_sections)
	{
		 
		old_entry->obj = pdf_deep_copy_obj(ctx, old_entry->obj);
	}
	else
	{
		old_entry->obj = NULL;
	}
	old_entry->stm_buf = NULL;
}
