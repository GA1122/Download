pdf_drop_document(fz_context *ctx, pdf_document *doc)
{
	fz_drop_document(ctx, &doc->super);
}
