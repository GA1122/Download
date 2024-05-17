pdf_process_annot(fz_context *ctx, pdf_processor *proc, pdf_document *doc, pdf_page *page, pdf_annot *annot, fz_cookie *cookie)
{
	int flags = pdf_to_int(ctx, pdf_dict_get(ctx, annot->obj, PDF_NAME_F));

	if (flags & (PDF_ANNOT_IS_INVISIBLE | PDF_ANNOT_IS_HIDDEN))
		return;

	 
	if (pdf_annot_type(ctx, annot) == PDF_ANNOT_POPUP)
		return;

	if (proc->usage)
	{
		if (!strcmp(proc->usage, "Print") && !(flags & PDF_ANNOT_IS_PRINT))
			return;
		if (!strcmp(proc->usage, "View") && (flags & PDF_ANNOT_IS_NO_VIEW))
			return;
	}

	 

	 
	if (pdf_is_hidden_ocg(ctx, doc->ocg, NULL, proc->usage, pdf_dict_get(ctx, annot->obj, PDF_NAME_OC)))
		return;

	if (proc->op_q && proc->op_cm && proc->op_Do_form && proc->op_Q && annot->ap)
	{
		fz_matrix matrix;
		pdf_annot_transform(ctx, annot, &matrix);
		proc->op_q(ctx, proc);
		proc->op_cm(ctx, proc,
			matrix.a, matrix.b,
			matrix.c, matrix.d,
			matrix.e, matrix.f);
		proc->op_Do_form(ctx, proc, NULL, annot->ap, pdf_page_resources(ctx, page));
		proc->op_Q(ctx, proc);
	}
}
