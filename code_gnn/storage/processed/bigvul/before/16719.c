pdf_read_xref_sections(fz_context *ctx, pdf_document *doc, int64_t ofs, pdf_lexbuf *buf, int read_previous)
{
	ofs_list list;

	list.len = 0;
	list.max = 10;
	list.list = fz_malloc_array(ctx, 10, sizeof(*list.list));
	fz_try(ctx)
	{
		while(ofs)
		{
			pdf_populate_next_xref_level(ctx, doc);
			ofs = read_xref_section(ctx, doc, ofs, buf, &list);
			if (!read_previous)
				break;
		}
	}
	fz_always(ctx)
	{
		fz_free(ctx, list.list);
	}
	fz_catch(ctx)
	{
		fz_rethrow(ctx);
	}
}
