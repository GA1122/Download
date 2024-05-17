pdf_cache_object(fz_context *ctx, pdf_document *doc, int num)
{
	pdf_xref_entry *x;
	int rnum, rgen, try_repair;

	fz_var(try_repair);

	if (num <= 0 || num >= pdf_xref_len(ctx, doc))
		fz_throw(ctx, FZ_ERROR_GENERIC, "object out of range (%d 0 R); xref size %d", num, pdf_xref_len(ctx, doc));

object_updated:
	try_repair = 0;
	rnum = num;

	x = pdf_get_xref_entry(ctx, doc, num);

	if (x->obj != NULL)
		return x;

	if (x->type == 'f')
	{
		x->obj = pdf_new_null(ctx, doc);
	}
	else if (x->type == 'n')
	{
		fz_seek(ctx, doc->file, x->ofs, SEEK_SET);

		fz_try(ctx)
		{
			x->obj = pdf_parse_ind_obj(ctx, doc, doc->file, &doc->lexbuf.base,
					&rnum, &rgen, &x->stm_ofs, &try_repair);
		}
		fz_catch(ctx)
		{
			if (!try_repair || fz_caught(ctx) == FZ_ERROR_TRYLATER)
				fz_rethrow(ctx);
		}

		if (!try_repair && rnum != num)
		{
			pdf_drop_obj(ctx, x->obj);
			x->type = 'f';
			x->ofs = -1;
			x->gen = 0;
			x->num = 0;
			x->stm_ofs = 0;
			x->obj = NULL;
			try_repair = (doc->repair_attempted == 0);
		}

		if (try_repair)
		{
			fz_try(ctx)
			{
				pdf_repair_xref(ctx, doc);
				pdf_prime_xref_index(ctx, doc);
				pdf_repair_obj_stms(ctx, doc);
			}
			fz_catch(ctx)
			{
				if (rnum == num)
					fz_throw(ctx, FZ_ERROR_GENERIC, "cannot parse object (%d 0 R)", num);
				else
					fz_throw(ctx, FZ_ERROR_GENERIC, "found object (%d 0 R) instead of (%d 0 R)", rnum, num);
			}
			goto object_updated;
		}

		if (doc->crypt)
			pdf_crypt_obj(ctx, doc->crypt, x->obj, x->num, x->gen);
	}
	else if (x->type == 'o')
	{
		if (!x->obj)
		{
			x = pdf_load_obj_stm(ctx, doc, x->ofs, &doc->lexbuf.base, num);
			if (x == NULL)
				fz_throw(ctx, FZ_ERROR_GENERIC, "cannot load object stream containing object (%d 0 R)", num);
			if (!x->obj)
				fz_throw(ctx, FZ_ERROR_GENERIC, "object (%d 0 R) was not found in its object stream", num);
		}
	}
	else if (doc->hint_obj_offsets && read_hinted_object(ctx, doc, num))
	{
		goto object_updated;
	}
	else if (doc->file_length && doc->linear_pos < doc->file_length)
	{
		fz_throw(ctx, FZ_ERROR_TRYLATER, "cannot find object in xref (%d 0 R) - not loaded yet?", num);
	}
	else
	{
		fz_throw(ctx, FZ_ERROR_GENERIC, "cannot find object in xref (%d 0 R)", num);
	}

	pdf_set_obj_parent(ctx, x->obj, num);
	return x;
}
