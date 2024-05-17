parse_inline_image(fz_context *ctx, pdf_csi *csi, fz_stream *stm)
{
	pdf_document *doc = csi->doc;
	pdf_obj *rdb = csi->rdb;
	pdf_obj *obj = NULL;
	fz_image *img = NULL;
	int ch, found;

	fz_var(obj);
	fz_var(img);

	fz_try(ctx)
	{
		obj = pdf_parse_dict(ctx, doc, stm, &doc->lexbuf.base);

		 
		ch = fz_read_byte(ctx, stm);
		if (ch == '\r')
			if (fz_peek_byte(ctx, stm) == '\n')
				fz_read_byte(ctx, stm);

		img = pdf_load_inline_image(ctx, doc, rdb, obj, stm);

		 
		found = 0;
		ch = fz_read_byte(ctx, stm);
		do
		{
			while (ch != 'E' && ch != EOF)
				ch = fz_read_byte(ctx, stm);
			if (ch == 'E')
			{
				ch = fz_read_byte(ctx, stm);
				if (ch == 'I')
				{
					ch = fz_peek_byte(ctx, stm);
					if (ch == ' ' || ch <= 32 || ch == EOF || ch == '<' || ch == '/')
					{
						found = 1;
						break;
					}
				}
			}
		} while (ch != EOF);
		if (!found)
			fz_throw(ctx, FZ_ERROR_SYNTAX, "syntax error after inline image");
	}
	fz_always(ctx)
	{
		pdf_drop_obj(ctx, obj);
	}
	fz_catch(ctx)
	{
		fz_drop_image(ctx, img);
		fz_rethrow(ctx);
	}

	return img;
}
