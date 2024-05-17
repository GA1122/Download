pdf_init_document(fz_context *ctx, pdf_document *doc)
{
	pdf_obj *encrypt, *id;
	pdf_obj *dict = NULL;
	pdf_obj *obj;
	pdf_obj *nobj = NULL;
	int i, repaired = 0;

	fz_var(dict);
	fz_var(nobj);

	fz_try(ctx)
	{
		pdf_load_version(ctx, doc);

		doc->file_length = fz_stream_meta(ctx, doc->file, FZ_STREAM_META_LENGTH, 0, NULL);
		if (doc->file_length < 0)
			doc->file_length = 0;

		 
		if (fz_stream_meta(ctx, doc->file, FZ_STREAM_META_PROGRESSIVE, 0, NULL) > 0)
			doc->file_reading_linearly = 1;

		 
		if (doc->file_reading_linearly)
			pdf_load_linear(ctx, doc);

		 
		if (!doc->file_reading_linearly)
			pdf_load_xref(ctx, doc, &doc->lexbuf.base);
	}
	fz_catch(ctx)
	{
		pdf_drop_xref_sections(ctx, doc);
		fz_rethrow_if(ctx, FZ_ERROR_TRYLATER);
		fz_warn(ctx, "trying to repair broken xref");
		repaired = 1;
	}

	fz_try(ctx)
	{
		int hasroot, hasinfo;

		if (repaired)
		{
			 
			memset(doc->xref_index, 0, sizeof(int) * doc->max_xref_len);
			pdf_repair_xref(ctx, doc);
			pdf_prime_xref_index(ctx, doc);
		}

		encrypt = pdf_dict_get(ctx, pdf_trailer(ctx, doc), PDF_NAME_Encrypt);
		id = pdf_dict_get(ctx, pdf_trailer(ctx, doc), PDF_NAME_ID);
		if (pdf_is_dict(ctx, encrypt))
			doc->crypt = pdf_new_crypt(ctx, encrypt, id);

		 
		pdf_authenticate_password(ctx, doc, "");

		if (repaired)
		{
			int xref_len = pdf_xref_len(ctx, doc);
			pdf_repair_obj_stms(ctx, doc);

			hasroot = (pdf_dict_get(ctx, pdf_trailer(ctx, doc), PDF_NAME_Root) != NULL);
			hasinfo = (pdf_dict_get(ctx, pdf_trailer(ctx, doc), PDF_NAME_Info) != NULL);

			for (i = 1; i < xref_len; i++)
			{
				pdf_xref_entry *entry = pdf_get_xref_entry(ctx, doc, i);
				if (entry->type == 0 || entry->type == 'f')
					continue;

				fz_try(ctx)
				{
					dict = pdf_load_object(ctx, doc, i);
				}
				fz_catch(ctx)
				{
					fz_rethrow_if(ctx, FZ_ERROR_TRYLATER);
					fz_warn(ctx, "ignoring broken object (%d 0 R)", i);
					continue;
				}

				if (!hasroot)
				{
					obj = pdf_dict_get(ctx, dict, PDF_NAME_Type);
					if (pdf_name_eq(ctx, obj, PDF_NAME_Catalog))
					{
						nobj = pdf_new_indirect(ctx, doc, i, 0);
						pdf_dict_put_drop(ctx, pdf_trailer(ctx, doc), PDF_NAME_Root, nobj);
					}
				}

				if (!hasinfo)
				{
					if (pdf_dict_get(ctx, dict, PDF_NAME_Creator) || pdf_dict_get(ctx, dict, PDF_NAME_Producer))
					{
						nobj = pdf_new_indirect(ctx, doc, i, 0);
						pdf_dict_put_drop(ctx, pdf_trailer(ctx, doc), PDF_NAME_Info, nobj);
					}
				}

				pdf_drop_obj(ctx, dict);
				dict = NULL;
			}

			 
			if (doc->crypt)
				pdf_clear_xref(ctx, doc);
		}
	}
	fz_catch(ctx)
	{
		pdf_drop_obj(ctx, dict);
		fz_rethrow(ctx);
	}

	fz_try(ctx)
	{
		pdf_read_ocg(ctx, doc);
	}
	fz_catch(ctx)
	{
		fz_warn(ctx, "Ignoring Broken Optional Content");
	}

	fz_try(ctx)
	{
		const char *version_str;
		obj = pdf_dict_getl(ctx, pdf_trailer(ctx, doc), PDF_NAME_Root, PDF_NAME_Version, NULL);
		version_str = pdf_to_name(ctx, obj);
		if (*version_str)
		{
			int version = 10 * (fz_atof(version_str) + 0.05f);
			if (version > doc->version)
				doc->version = version;
		}
	}
	fz_catch(ctx) { }
}
