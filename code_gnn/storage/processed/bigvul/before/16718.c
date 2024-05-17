pdf_read_xref(fz_context *ctx, pdf_document *doc, int64_t ofs, pdf_lexbuf *buf)
{
	pdf_obj *trailer;
	int c;

	fz_seek(ctx, doc->file, ofs, SEEK_SET);

	while (iswhite(fz_peek_byte(ctx, doc->file)))
		fz_read_byte(ctx, doc->file);

	c = fz_peek_byte(ctx, doc->file);
	if (c == 'x')
		trailer = pdf_read_old_xref(ctx, doc, buf);
	else if (c >= '0' && c <= '9')
		trailer = pdf_read_new_xref(ctx, doc, buf);
	else
		fz_throw(ctx, FZ_ERROR_GENERIC, "cannot recognize xref format");

	return trailer;
}
