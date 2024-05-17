pdf_tos_newline(pdf_text_object_state *tos, float leading)
{
	fz_pre_translate(&tos->tlm, 0, -leading);
	tos->tm = tos->tlm;
}
