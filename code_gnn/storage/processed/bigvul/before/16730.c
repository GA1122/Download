int pdf_xref_obj_is_unsaved_signature(pdf_document *doc, pdf_obj *obj)
{
	int i;
	for (i = 0; i < doc->num_incremental_sections; i++)
	{
		pdf_xref *xref = &doc->xref_sections[i];
		pdf_unsaved_sig *usig;

		for (usig = xref->unsaved_sigs; usig; usig = usig->next)
		{
			if (usig->field == obj)
				return 1;
		}
	}

	return 0;
}
