static void pdf_populate_next_xref_level(fz_context *ctx, pdf_document *doc)
{
	pdf_xref *xref;
	doc->xref_sections = fz_resize_array(ctx, doc->xref_sections, doc->num_xref_sections + 1, sizeof(pdf_xref));
	doc->num_xref_sections++;

	xref = &doc->xref_sections[doc->num_xref_sections - 1];
	xref->subsec = NULL;
	xref->num_objects = 0;
	xref->trailer = NULL;
	xref->pre_repair_trailer = NULL;
	xref->unsaved_sigs = NULL;
	xref->unsaved_sigs_end = NULL;
}
