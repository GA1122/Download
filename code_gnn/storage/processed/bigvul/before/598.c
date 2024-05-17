pdf_tos_set_matrix(pdf_text_object_state *tos, float a, float b, float c, float d, float e, float f)
{
	tos->tm.a = a;
	tos->tm.b = b;
	tos->tm.c = c;
	tos->tm.d = d;
	tos->tm.e = e;
	tos->tm.f = f;
	tos->tlm = tos->tm;
}
