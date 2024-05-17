void pdf_xref_store_unsaved_signature(fz_context *ctx, pdf_document *doc, pdf_obj *field, pdf_signer *signer)
{
	pdf_xref *xref = &doc->xref_sections[0];
	pdf_unsaved_sig *unsaved_sig;

	 
	unsaved_sig = fz_malloc_struct(ctx, pdf_unsaved_sig);
	unsaved_sig->field = pdf_keep_obj(ctx, field);
	unsaved_sig->signer = pdf_keep_signer(ctx, signer);
	unsaved_sig->next = NULL;
	if (xref->unsaved_sigs_end == NULL)
		xref->unsaved_sigs_end = &xref->unsaved_sigs;

	*xref->unsaved_sigs_end = unsaved_sig;
	xref->unsaved_sigs_end = &unsaved_sig->next;
}
