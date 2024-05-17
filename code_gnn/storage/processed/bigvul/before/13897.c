pdf_repair_obj(fz_context *ctx, pdf_document *doc, pdf_lexbuf *buf, fz_off_t *stmofsp, int *stmlenp, pdf_obj **encrypt, pdf_obj **id, pdf_obj **page, fz_off_t *tmpofs, pdf_obj **root)
{
	fz_stream *file = doc->file;
	pdf_token tok;
	int stm_len;

	*stmofsp = 0;
	if (stmlenp)
		*stmlenp = -1;

	stm_len = 0;

	 
	tok = pdf_lex(ctx, file, buf);

	if (tok == PDF_TOK_OPEN_DICT)
	{
		pdf_obj *dict, *obj;

		fz_try(ctx)
		{
			dict = pdf_parse_dict(ctx, doc, file, buf);
		}
		fz_catch(ctx)
		{
			fz_rethrow_if(ctx, FZ_ERROR_TRYLATER);
			 
			if (file->eof)
				fz_rethrow(ctx);
			 
			dict = pdf_new_dict(ctx, NULL, 2);
		}

		 

		if (encrypt || id || root)
		{
			obj = pdf_dict_get(ctx, dict, PDF_NAME_Type);
			if (!pdf_is_indirect(ctx, obj) && pdf_name_eq(ctx, obj, PDF_NAME_XRef))
			{
				if (encrypt)
				{
					obj = pdf_dict_get(ctx, dict, PDF_NAME_Encrypt);
					if (obj)
					{
						pdf_drop_obj(ctx, *encrypt);
						*encrypt = pdf_keep_obj(ctx, obj);
					}
				}

				if (id)
				{
					obj = pdf_dict_get(ctx, dict, PDF_NAME_ID);
					if (obj)
					{
						pdf_drop_obj(ctx, *id);
						*id = pdf_keep_obj(ctx, obj);
					}
				}

				if (root)
					*root = pdf_keep_obj(ctx, pdf_dict_get(ctx, dict, PDF_NAME_Root));
			}
		}

		obj = pdf_dict_get(ctx, dict, PDF_NAME_Length);
		if (!pdf_is_indirect(ctx, obj) && pdf_is_int(ctx, obj))
			stm_len = pdf_to_int(ctx, obj);

		if (doc->file_reading_linearly && page)
		{
			obj = pdf_dict_get(ctx, dict, PDF_NAME_Type);
			if (!pdf_is_indirect(ctx, obj) && pdf_name_eq(ctx, obj, PDF_NAME_Page))
			{
				pdf_drop_obj(ctx, *page);
				*page = pdf_keep_obj(ctx, dict);
			}
		}

		pdf_drop_obj(ctx, dict);
	}

	while ( tok != PDF_TOK_STREAM &&
		tok != PDF_TOK_ENDOBJ &&
		tok != PDF_TOK_ERROR &&
		tok != PDF_TOK_EOF &&
		tok != PDF_TOK_INT )
	{
		*tmpofs = fz_tell(ctx, file);
		if (*tmpofs < 0)
			fz_throw(ctx, FZ_ERROR_GENERIC, "cannot tell in file");
		tok = pdf_lex(ctx, file, buf);
	}

	if (tok == PDF_TOK_STREAM)
	{
		int c = fz_read_byte(ctx, file);
		if (c == '\r') {
			c = fz_peek_byte(ctx, file);
			if (c == '\n')
				fz_read_byte(ctx, file);
		}

		*stmofsp = fz_tell(ctx, file);
		if (*stmofsp < 0)
			fz_throw(ctx, FZ_ERROR_GENERIC, "cannot seek in file");

		if (stm_len > 0)
		{
			fz_seek(ctx, file, *stmofsp + stm_len, 0);
			fz_try(ctx)
			{
				tok = pdf_lex(ctx, file, buf);
			}
			fz_catch(ctx)
			{
				fz_rethrow_if(ctx, FZ_ERROR_TRYLATER);
				fz_warn(ctx, "cannot find endstream token, falling back to scanning");
			}
			if (tok == PDF_TOK_ENDSTREAM)
				goto atobjend;
			fz_seek(ctx, file, *stmofsp, 0);
		}

		(void)fz_read(ctx, file, (unsigned char *) buf->scratch, 9);

		while (memcmp(buf->scratch, "endstream", 9) != 0)
		{
			c = fz_read_byte(ctx, file);
			if (c == EOF)
				break;
			memmove(&buf->scratch[0], &buf->scratch[1], 8);
			buf->scratch[8] = c;
		}

		if (stmlenp)
			*stmlenp = fz_tell(ctx, file) - *stmofsp - 9;

atobjend:
		*tmpofs = fz_tell(ctx, file);
		if (*tmpofs < 0)
			fz_throw(ctx, FZ_ERROR_GENERIC, "cannot tell in file");
		tok = pdf_lex(ctx, file, buf);
		if (tok != PDF_TOK_ENDOBJ)
			fz_warn(ctx, "object missing 'endobj' token");
		else
		{
			 
			*tmpofs = fz_tell(ctx, file);
			if (*tmpofs < 0)
				fz_throw(ctx, FZ_ERROR_GENERIC, "cannot tell in file");
			tok = pdf_lex(ctx, file, buf);
		}
	}
	return tok;
}
