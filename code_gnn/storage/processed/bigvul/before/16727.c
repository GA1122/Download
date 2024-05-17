pdf_xref_find_subsection(fz_context *ctx, pdf_document *doc, int64_t ofs, int len)
{
	pdf_xref *xref = &doc->xref_sections[doc->num_xref_sections-1];
	pdf_xref_subsec *sub;
	int new_max;

	 

	 
	for (sub = xref->subsec; sub != NULL; sub = sub->next)
	{
		if (ofs >= sub->start && ofs + len <= sub->start + sub->len)
			return &sub->table[ofs-sub->start];  
		if (ofs + len > sub->start && ofs <= sub->start + sub->len)
			break;  
	}

	new_max = xref->num_objects;
	if (new_max < ofs + len)
		new_max = ofs + len;

	if (sub == NULL)
	{
		 
		sub = fz_malloc_struct(ctx, pdf_xref_subsec);
		fz_try(ctx)
		{
			sub->table = fz_calloc(ctx, len, sizeof(pdf_xref_entry));
			sub->start = ofs;
			sub->len = len;
			sub->next = xref->subsec;
			xref->subsec = sub;
		}
		fz_catch(ctx)
		{
			fz_free(ctx, sub);
			fz_rethrow(ctx);
		}
		xref->num_objects = new_max;
		if (doc->max_xref_len < new_max)
			extend_xref_index(ctx, doc, new_max);
	}
	else
	{
		 
		ensure_solid_xref(ctx, doc, new_max, doc->num_xref_sections-1);
		xref = &doc->xref_sections[doc->num_xref_sections-1];
		sub = xref->subsec;
	}
	return &sub->table[ofs-sub->start];
}
