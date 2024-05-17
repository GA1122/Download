pdf_load_linear(fz_context *ctx, pdf_document *doc)
{
	pdf_obj *dict = NULL;
	pdf_obj *hint = NULL;
	pdf_obj *o;
	int num, gen, lin, len;
	int64_t stmofs;

	fz_var(dict);
	fz_var(hint);

	fz_try(ctx)
	{
		pdf_xref_entry *entry;

		dict = pdf_parse_ind_obj(ctx, doc, doc->file, &doc->lexbuf.base, &num, &gen, &stmofs, NULL);
		if (!pdf_is_dict(ctx, dict))
			fz_throw(ctx, FZ_ERROR_GENERIC, "Failed to read linearized dictionary");
		o = pdf_dict_get(ctx, dict, PDF_NAME_Linearized);
		if (o == NULL)
			fz_throw(ctx, FZ_ERROR_GENERIC, "Failed to read linearized dictionary");
		lin = pdf_to_int(ctx, o);
		if (lin != 1)
			fz_throw(ctx, FZ_ERROR_GENERIC, "Unexpected version of Linearized tag (%d)", lin);
		len = pdf_to_int(ctx, pdf_dict_get(ctx, dict, PDF_NAME_L));
		if (len != doc->file_length)
			fz_throw(ctx, FZ_ERROR_GENERIC, "File has been updated since linearization");

		pdf_read_xref_sections(ctx, doc, fz_tell(ctx, doc->file), &doc->lexbuf.base, 0);

		doc->linear_page_count = pdf_to_int(ctx, pdf_dict_get(ctx, dict, PDF_NAME_N));
		doc->linear_page_refs = fz_resize_array(ctx, doc->linear_page_refs, doc->linear_page_count, sizeof(pdf_obj *));
		memset(doc->linear_page_refs, 0, doc->linear_page_count * sizeof(pdf_obj*));
		doc->linear_obj = dict;
		doc->linear_pos = fz_tell(ctx, doc->file);
		doc->linear_page1_obj_num = pdf_to_int(ctx, pdf_dict_get(ctx, dict, PDF_NAME_O));
		doc->linear_page_refs[0] = pdf_new_indirect(ctx, doc, doc->linear_page1_obj_num, 0);
		doc->linear_page_num = 0;
		hint = pdf_dict_get(ctx, dict, PDF_NAME_H);
		doc->hint_object_offset = pdf_to_int(ctx, pdf_array_get(ctx, hint, 0));
		doc->hint_object_length = pdf_to_int(ctx, pdf_array_get(ctx, hint, 1));

		entry = pdf_get_populating_xref_entry(ctx, doc, 0);
		entry->type = 'f';
	}
	fz_catch(ctx)
	{
		pdf_drop_obj(ctx, dict);
		fz_rethrow_if(ctx, FZ_ERROR_TRYLATER);
		 
		doc->file_reading_linearly = 0;
	}
}
