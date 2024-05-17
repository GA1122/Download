pdf_read_new_xref(fz_context *ctx, pdf_document *doc, pdf_lexbuf *buf)
{
	fz_stream *stm = NULL;
	pdf_obj *trailer = NULL;
	pdf_obj *index = NULL;
	pdf_obj *obj = NULL;
	int gen, num = 0;
	int64_t ofs, stm_ofs;
	int size, w0, w1, w2;
	int t;

	fz_var(trailer);
	fz_var(stm);

	fz_try(ctx)
	{
		ofs = fz_tell(ctx, doc->file);
		trailer = pdf_parse_ind_obj(ctx, doc, doc->file, buf, &num, &gen, &stm_ofs, NULL);
	}
	fz_catch(ctx)
	{
		pdf_drop_obj(ctx, trailer);
		fz_rethrow(ctx);
	}

	fz_try(ctx)
	{
		pdf_xref_entry *entry;

		obj = pdf_dict_get(ctx, trailer, PDF_NAME_Size);
		if (!obj)
			fz_throw(ctx, FZ_ERROR_GENERIC, "xref stream missing Size entry (%d 0 R)", num);

		size = pdf_to_int(ctx, obj);

		obj = pdf_dict_get(ctx, trailer, PDF_NAME_W);
		if (!obj)
			fz_throw(ctx, FZ_ERROR_GENERIC, "xref stream missing W entry (%d  R)", num);
		w0 = pdf_to_int(ctx, pdf_array_get(ctx, obj, 0));
		w1 = pdf_to_int(ctx, pdf_array_get(ctx, obj, 1));
		w2 = pdf_to_int(ctx, pdf_array_get(ctx, obj, 2));

		if (w0 < 0)
			fz_warn(ctx, "xref stream objects have corrupt type");
		if (w1 < 0)
			fz_warn(ctx, "xref stream objects have corrupt offset");
		if (w2 < 0)
			fz_warn(ctx, "xref stream objects have corrupt generation");

		w0 = w0 < 0 ? 0 : w0;
		w1 = w1 < 0 ? 0 : w1;
		w2 = w2 < 0 ? 0 : w2;

		index = pdf_dict_get(ctx, trailer, PDF_NAME_Index);

		stm = pdf_open_stream_with_offset(ctx, doc, num, trailer, stm_ofs);

		if (!index)
		{
			pdf_read_new_xref_section(ctx, doc, stm, 0, size, w0, w1, w2);
		}
		else
		{
			int n = pdf_array_len(ctx, index);
			for (t = 0; t < n; t += 2)
			{
				int i0 = pdf_to_int(ctx, pdf_array_get(ctx, index, t + 0));
				int i1 = pdf_to_int(ctx, pdf_array_get(ctx, index, t + 1));
				pdf_read_new_xref_section(ctx, doc, stm, i0, i1, w0, w1, w2);
			}
		}
		entry = pdf_get_populating_xref_entry(ctx, doc, num);
		entry->ofs = ofs;
		entry->gen = gen;
		entry->num = num;
		entry->stm_ofs = stm_ofs;
		pdf_drop_obj(ctx, entry->obj);
		entry->obj = pdf_keep_obj(ctx, trailer);
		entry->type = 'n';
	}
	fz_always(ctx)
	{
		fz_drop_stream(ctx, stm);
	}
	fz_catch(ctx)
	{
		pdf_drop_obj(ctx, trailer);
		fz_rethrow(ctx);
	}

	return trailer;
}
