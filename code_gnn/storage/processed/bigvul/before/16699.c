static void pdf_drop_xref_sections(fz_context *ctx, pdf_document *doc)
{
	pdf_drop_xref_sections_imp(ctx, doc, doc->saved_xref_sections, doc->saved_num_xref_sections);
	pdf_drop_xref_sections_imp(ctx, doc, doc->xref_sections, doc->num_xref_sections);

	doc->saved_xref_sections = NULL;
	doc->saved_num_xref_sections = 0;
	doc->xref_sections = NULL;
	doc->num_xref_sections = 0;
	doc->num_incremental_sections = 0;
}
