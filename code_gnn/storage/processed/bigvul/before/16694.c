pdf_repair_obj_stm(fz_context *ctx, pdf_document *doc, int stm_num)
{
	pdf_obj *obj;
	fz_stream *stm = NULL;
	pdf_token tok;
	int i, n, count;
	pdf_lexbuf buf;

	fz_var(stm);

	pdf_lexbuf_init(ctx, &buf, PDF_LEXBUF_SMALL);

	fz_try(ctx)
	{
		obj = pdf_load_object(ctx, doc, stm_num);

		count = pdf_to_int(ctx, pdf_dict_get(ctx, obj, PDF_NAME_N));

		pdf_drop_obj(ctx, obj);

		stm = pdf_open_stream_number(ctx, doc, stm_num);

		for (i = 0; i < count; i++)
		{
			pdf_xref_entry *entry;

			tok = pdf_lex(ctx, stm, &buf);
			if (tok != PDF_TOK_INT)
				fz_throw(ctx, FZ_ERROR_GENERIC, "corrupt object stream (%d 0 R)", stm_num);

			n = buf.i;
			if (n < 0)
			{
				fz_warn(ctx, "ignoring object with invalid object number (%d %d R)", n, i);
				continue;
			}
			else if (n >= pdf_xref_len(ctx, doc))
			{
				fz_warn(ctx, "ignoring object with invalid object number (%d %d R)", n, i);
				continue;
			}

			entry = pdf_get_populating_xref_entry(ctx, doc, n);
			entry->ofs = stm_num;
			entry->gen = i;
			entry->num = n;
			entry->stm_ofs = 0;
			pdf_drop_obj(ctx, entry->obj);
			entry->obj = NULL;
			entry->type = 'o';

			tok = pdf_lex(ctx, stm, &buf);
			if (tok != PDF_TOK_INT)
				fz_throw(ctx, FZ_ERROR_GENERIC, "corrupt object stream (%d 0 R)", stm_num);
		}
	}
	fz_always(ctx)
	{
		fz_drop_stream(ctx, stm);
		pdf_lexbuf_fin(ctx, &buf);
	}
	fz_catch(ctx)
	{
		fz_rethrow(ctx);
	}
}
