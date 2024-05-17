static void pdf_resize_xref(fz_context *ctx, pdf_document *doc, int newlen)
{
	int i;
	pdf_xref *xref = &doc->xref_sections[doc->xref_base];
	pdf_xref_subsec *sub;

	assert(xref != NULL);
	sub = xref->subsec;
	assert(sub->next == NULL && sub->start == 0 && sub->len == xref->num_objects);
	assert(newlen > xref->num_objects);

	sub->table = fz_resize_array(ctx, sub->table, newlen, sizeof(pdf_xref_entry));
	for (i = xref->num_objects; i < newlen; i++)
	{
		sub->table[i].type = 0;
		sub->table[i].ofs = 0;
		sub->table[i].gen = 0;
		sub->table[i].num = 0;
		sub->table[i].stm_ofs = 0;
		sub->table[i].stm_buf = NULL;
		sub->table[i].obj = NULL;
	}
	xref->num_objects = newlen;
	sub->len = newlen;
	if (doc->max_xref_len < newlen)
		extend_xref_index(ctx, doc, newlen);
}
