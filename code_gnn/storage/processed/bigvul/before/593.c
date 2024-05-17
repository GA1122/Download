pdf_tos_move_after_char(fz_context *ctx, pdf_text_object_state *tos)
{
	fz_union_rect(&tos->text_bbox, &tos->char_bbox);

	fz_pre_translate(&tos->tm, tos->char_tx, tos->char_ty);
}
