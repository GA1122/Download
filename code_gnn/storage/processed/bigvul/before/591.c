pdf_tos_get_text(fz_context *ctx, pdf_text_object_state *tos)
{
	fz_text *text = tos->text;

	tos->text = NULL;

	return text;
}
