pdf_tos_restore(fz_context *ctx, pdf_text_object_state *tos, fz_matrix save[2])
{
	tos->tm = save[0];
	tos->tlm = save[1];
}
