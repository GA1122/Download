pdf_xref_size_from_old_trailer(fz_context *ctx, pdf_document *doc, pdf_lexbuf *buf)
{
	int64_t len;
	char *s;
	int64_t t;
	pdf_token tok;
	int c;
	int size = 0;
	int64_t ofs;
	pdf_obj *trailer = NULL;
	size_t n;

	fz_var(trailer);

	 
	ofs = fz_tell(ctx, doc->file);

	fz_skip_space(ctx, doc->file);
	if (fz_skip_string(ctx, doc->file, "xref"))
		fz_throw(ctx, FZ_ERROR_GENERIC, "cannot find xref marker");
	fz_skip_space(ctx, doc->file);

	while (1)
	{
		c = fz_peek_byte(ctx, doc->file);
		if (!(c >= '0' && c <= '9'))
			break;

		fz_read_line(ctx, doc->file, buf->scratch, buf->size);
		s = buf->scratch;
		fz_strsep(&s, " ");  
		if (!s)
			fz_throw(ctx, FZ_ERROR_GENERIC, "invalid range marker in xref");
		len = fz_atoi64(fz_strsep(&s, " "));
		if (len < 0)
			fz_throw(ctx, FZ_ERROR_GENERIC, "xref range marker must be positive");

		 
		if (s && *s != '\0')
			fz_seek(ctx, doc->file, -(2 + (int)strlen(s)), SEEK_CUR);

		t = fz_tell(ctx, doc->file);
		if (t < 0)
			fz_throw(ctx, FZ_ERROR_GENERIC, "cannot tell in file");

		 
		if (len > 0)
		{
			n = fz_read(ctx, doc->file, (unsigned char *)buf->scratch, 20);
			if (n < 19)
				fz_throw(ctx, FZ_ERROR_GENERIC, "malformed xref table");
			if (n == 20 && buf->scratch[19] > 32)
				n = 19;
		}
		else
			n = 20;

		if (len > (int64_t)((INT64_MAX - t) / n))
			fz_throw(ctx, FZ_ERROR_GENERIC, "xref has too many entries");

		fz_seek(ctx, doc->file, (int64_t)(t + n * len), SEEK_SET);
	}

	fz_try(ctx)
	{
		tok = pdf_lex(ctx, doc->file, buf);
		if (tok != PDF_TOK_TRAILER)
			fz_throw(ctx, FZ_ERROR_GENERIC, "expected trailer marker");

		tok = pdf_lex(ctx, doc->file, buf);
		if (tok != PDF_TOK_OPEN_DICT)
			fz_throw(ctx, FZ_ERROR_GENERIC, "expected trailer dictionary");

		trailer = pdf_parse_dict(ctx, doc, doc->file, buf);

		size = pdf_to_int(ctx, pdf_dict_get(ctx, trailer, PDF_NAME_Size));
		if (!size)
			fz_throw(ctx, FZ_ERROR_GENERIC, "trailer missing Size entry");
	}
	fz_always(ctx)
	{
		pdf_drop_obj(ctx, trailer);
	}
	fz_catch(ctx)
	{
		fz_rethrow(ctx);
	}

	fz_seek(ctx, doc->file, ofs, SEEK_SET);

	return size;
}
