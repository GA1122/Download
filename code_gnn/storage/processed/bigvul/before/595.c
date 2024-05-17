pdf_tos_reset(fz_context *ctx, pdf_text_object_state *tos, int render)
{
	tos->text = fz_new_text(ctx);
	tos->text_mode = render;
	tos->text_bbox = fz_empty_rect;
}
