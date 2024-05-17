pdf_tos_save(fz_context *ctx, pdf_text_object_state *tos, fz_matrix save[2])
{
	save[0] = tos->tm;
	save[1] = tos->tlm;
}
