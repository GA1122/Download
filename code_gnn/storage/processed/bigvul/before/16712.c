pdf_load_xref(fz_context *ctx, pdf_document *doc, pdf_lexbuf *buf)
{
	int i;
	int xref_len;
	pdf_xref_entry *entry;

	pdf_read_start_xref(ctx, doc);

	pdf_read_xref_sections(ctx, doc, doc->startxref, buf, 1);

	if (pdf_xref_len(ctx, doc) == 0)
		fz_throw(ctx, FZ_ERROR_GENERIC, "found xref was empty");

	pdf_prime_xref_index(ctx, doc);

	entry = pdf_get_xref_entry(ctx, doc, 0);
	 
	if (!entry->type)
	{
		entry->type = 'f';
		entry->gen = 65535;
		entry->num = 0;
	}
	 
	else if (entry->type != 'f')
		fz_warn(ctx, "first object in xref is not free");

	 
	xref_len = pdf_xref_len(ctx, doc);
	for (i = 0; i < xref_len; i++)
	{
		entry = pdf_get_xref_entry(ctx, doc, i);
		if (entry->type == 'n')
		{
			 
			if (entry->ofs == 0)
				entry->type = 'f';
			else if (entry->ofs <= 0 || entry->ofs >= doc->file_size)
				fz_throw(ctx, FZ_ERROR_GENERIC, "object offset out of range: %d (%d 0 R)", (int)entry->ofs, i);
		}
		if (entry->type == 'o')
		{
			 
			int64_t ofs = entry->ofs;
			if (ofs <= 0 || ofs >= xref_len || pdf_get_xref_entry(ctx, doc, ofs)->type != 'n')
				fz_throw(ctx, FZ_ERROR_GENERIC, "invalid reference to an objstm that does not exist: %d (%d 0 R)", (int)ofs, i);
		}
	}
}
