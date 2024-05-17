pdf_new_processor(fz_context *ctx, int size)
{
	return Memento_label(fz_calloc(ctx, 1, size), "pdf_processor");
}
