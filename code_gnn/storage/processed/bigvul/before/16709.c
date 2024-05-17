pdf_load_obj_stm(fz_context *ctx, pdf_document *doc, int num, pdf_lexbuf *buf, int target)
{
	fz_stream *stm = NULL;
	pdf_obj *objstm = NULL;
	int *numbuf = NULL;
	int64_t *ofsbuf = NULL;

	pdf_obj *obj;
	int64_t first;
	int count;
	int i;
	pdf_token tok;
	pdf_xref_entry *ret_entry = NULL;

	fz_var(numbuf);
	fz_var(ofsbuf);
	fz_var(objstm);
	fz_var(stm);

	fz_try(ctx)
	{
		objstm = pdf_load_object(ctx, doc, num);

		count = pdf_to_int(ctx, pdf_dict_get(ctx, objstm, PDF_NAME_N));
		first = pdf_to_int(ctx, pdf_dict_get(ctx, objstm, PDF_NAME_First));

		if (count < 0)
			fz_throw(ctx, FZ_ERROR_GENERIC, "negative number of objects in object stream");
		if (first < 0)
			fz_throw(ctx, FZ_ERROR_GENERIC, "first object in object stream resides outside stream");

		numbuf = fz_calloc(ctx, count, sizeof(*numbuf));
		ofsbuf = fz_calloc(ctx, count, sizeof(*ofsbuf));

		stm = pdf_open_stream_number(ctx, doc, num);
		for (i = 0; i < count; i++)
		{
			tok = pdf_lex(ctx, stm, buf);
			if (tok != PDF_TOK_INT)
				fz_throw(ctx, FZ_ERROR_GENERIC, "corrupt object stream (%d 0 R)", num);
			numbuf[i] = buf->i;

			tok = pdf_lex(ctx, stm, buf);
			if (tok != PDF_TOK_INT)
				fz_throw(ctx, FZ_ERROR_GENERIC, "corrupt object stream (%d 0 R)", num);
			ofsbuf[i] = buf->i;
		}

		fz_seek(ctx, stm, first, SEEK_SET);

		for (i = 0; i < count; i++)
		{
			int xref_len = pdf_xref_len(ctx, doc);
			pdf_xref_entry *entry;
			fz_seek(ctx, stm, first + ofsbuf[i], SEEK_SET);

			obj = pdf_parse_stm_obj(ctx, doc, stm, buf);

			if (numbuf[i] <= 0 || numbuf[i] >= xref_len)
			{
				pdf_drop_obj(ctx, obj);
				fz_throw(ctx, FZ_ERROR_GENERIC, "object id (%d 0 R) out of range (0..%d)", numbuf[i], xref_len - 1);
			}

			entry = pdf_get_xref_entry(ctx, doc, numbuf[i]);

			pdf_set_obj_parent(ctx, obj, numbuf[i]);

			if (entry->type == 'o' && entry->ofs == num)
			{
				 
				if (entry->obj)
				{
					if (pdf_objcmp(ctx, entry->obj, obj))
						fz_warn(ctx, "Encountered new definition for object %d - keeping the original one", numbuf[i]);
					pdf_drop_obj(ctx, obj);
				}
				else
				{
					entry->obj = obj;
					fz_drop_buffer(ctx, entry->stm_buf);
					entry->stm_buf = NULL;
				}
				if (numbuf[i] == target)
					ret_entry = entry;
			}
			else
			{
				pdf_drop_obj(ctx, obj);
			}
		}
	}
	fz_always(ctx)
	{
		fz_drop_stream(ctx, stm);
		fz_free(ctx, ofsbuf);
		fz_free(ctx, numbuf);
		pdf_drop_obj(ctx, objstm);
	}
	fz_catch(ctx)
	{
		fz_rethrow(ctx);
	}
	return ret_entry;
}
