pdf_obj_read(fz_context *ctx, pdf_document *doc, int64_t *offset, int *nump, pdf_obj **page)
{
	pdf_lexbuf *buf = &doc->lexbuf.base;
	int num, gen, tok;
	int64_t numofs, genofs, stmofs, tmpofs, newtmpofs;
	int xref_len;
	pdf_xref_entry *entry;

	numofs = *offset;
	fz_seek(ctx, doc->file, numofs, SEEK_SET);

	 
	tok = pdf_lex(ctx, doc->file, buf);
	genofs = fz_tell(ctx, doc->file);
	if (tok != PDF_TOK_INT)
	{
		 
		DEBUGMESS((ctx, "skipping unexpected data (tok=%d) at %d", tok, *offset));
		*offset = genofs;
		return tok == PDF_TOK_EOF;
	}
	*nump = num = buf->i;

	 
	tok = pdf_lex(ctx, doc->file, buf);
	tmpofs = fz_tell(ctx, doc->file);
	if (tok != PDF_TOK_INT)
	{
		 
		DEBUGMESS((ctx, "skipping unexpected data after \"%d\" (tok=%d) at %d", num, tok, *offset));
		*offset = tmpofs;
		return tok == PDF_TOK_EOF;
	}
	gen = buf->i;

	 
	do
	{
		tmpofs = fz_tell(ctx, doc->file);
		tok = pdf_lex(ctx, doc->file, buf);
		if (tok == PDF_TOK_OBJ)
			break;
		if (tok != PDF_TOK_INT)
		{
			DEBUGMESS((ctx, "skipping unexpected data (tok=%d) at %d", tok, tmpofs));
			*offset = fz_tell(ctx, doc->file);
			return tok == PDF_TOK_EOF;
		}
		DEBUGMESS((ctx, "skipping unexpected int %d at %d", num, numofs));
		*nump = num = gen;
		numofs = genofs;
		gen = buf->i;
		genofs = tmpofs;
	}
	while (1);

	 
	xref_len = pdf_xref_len(ctx, doc);

	 
	tok = pdf_repair_obj(ctx, doc, buf, &stmofs, NULL, NULL, NULL, page, &newtmpofs, NULL);

	do  
	{
		if (num <= 0 || num >= xref_len)
		{
			fz_warn(ctx, "Not a valid object number (%d %d obj)", num, gen);
			break;
		}
		if (gen != 0)
		{
			fz_warn(ctx, "Unexpected non zero generation number in linearized file");
		}
		entry = pdf_get_populating_xref_entry(ctx, doc, num);
		if (entry->type != 0)
		{
			DEBUGMESS((ctx, "Duplicate object found (%d %d obj)", num, gen));
			break;
		}
		if (page && *page)
		{
			DEBUGMESS((ctx, "Successfully read object %d @ %d - and found page %d!", num, numofs, doc->linear_page_num));
			if (!entry->obj)
				entry->obj = pdf_keep_obj(ctx, *page);

			if (doc->linear_page_refs[doc->linear_page_num] == NULL)
				doc->linear_page_refs[doc->linear_page_num] = pdf_new_indirect(ctx, doc, num, gen);
		}
		else
		{
			DEBUGMESS((ctx, "Successfully read object %d @ %d", num, numofs));
		}
		entry->type = 'n';
		entry->gen = gen;  
		entry->num = num;
		entry->ofs = numofs;
		entry->stm_ofs = stmofs;
	}
	while (0);
	if (page && *page)
		doc->linear_page_num++;

	if (tok == PDF_TOK_ENDOBJ)
	{
		*offset = fz_tell(ctx, doc->file);
	}
	else
	{
		*offset = newtmpofs;
	}
	return 0;
}
