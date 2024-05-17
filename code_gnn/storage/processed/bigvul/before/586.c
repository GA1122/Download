pdf_process_glyph(fz_context *ctx, pdf_processor *proc, pdf_document *doc, pdf_obj *rdb, fz_buffer *contents)
{
	pdf_csi csi;
	pdf_lexbuf buf;
	fz_stream *stm = NULL;

	fz_var(stm);

	if (!contents)
		return;

	pdf_lexbuf_init(ctx, &buf, PDF_LEXBUF_SMALL);
	pdf_init_csi(ctx, &csi, doc, rdb, &buf, NULL);

	fz_try(ctx)
	{
		stm = fz_open_buffer(ctx, contents);
		pdf_process_stream(ctx, proc, &csi, stm);
		pdf_process_end(ctx, proc, &csi);
	}
	fz_always(ctx)
	{
		fz_drop_stream(ctx, stm);
		pdf_clear_stack(ctx, &csi);
		pdf_lexbuf_fin(ctx, &buf);
	}
	fz_catch(ctx)
	{
		fz_rethrow(ctx);
	}
}
