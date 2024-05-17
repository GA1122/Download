pdf_tos_translate(pdf_text_object_state *tos, float tx, float ty)
{
	fz_pre_translate(&tos->tlm, tx, ty);
	tos->tm = tos->tlm;
}
