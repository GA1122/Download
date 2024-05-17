extend_xref_index(fz_context *ctx, pdf_document *doc, int newlen)
{
	int i;

	doc->xref_index = fz_resize_array(ctx, doc->xref_index, newlen, sizeof(int));
	for (i = doc->max_xref_len; i < newlen; i++)
	{
		doc->xref_index[i] = 0;
	}
	doc->max_xref_len = newlen;
}
