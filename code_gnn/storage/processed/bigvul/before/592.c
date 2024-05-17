pdf_tos_make_trm(fz_context *ctx, pdf_text_object_state *tos, pdf_text_state *text, pdf_font_desc *fontdesc, int cid, fz_matrix *trm)
{
	fz_matrix tsm;

	tsm.a = text->size * text->scale;
	tsm.b = 0;
	tsm.c = 0;
	tsm.d = text->size;
	tsm.e = 0;
	tsm.f = text->rise;

	if (fontdesc->wmode == 0)
	{
		pdf_hmtx h = pdf_lookup_hmtx(ctx, fontdesc, cid);
		float w0 = h.w * 0.001f;
		tos->char_tx = (w0 * text->size + text->char_space) * text->scale;
		tos->char_ty = 0;
	}

	if (fontdesc->wmode == 1)
	{
		pdf_vmtx v = pdf_lookup_vmtx(ctx, fontdesc, cid);
		float w1 = v.w * 0.001f;
		tsm.e -= v.x * fabsf(text->size) * 0.001f;
		tsm.f -= v.y * text->size * 0.001f;
		tos->char_tx = 0;
		tos->char_ty = w1 * text->size + text->char_space;
	}

	fz_concat(trm, &tsm, &tos->tm);

	tos->cid = cid;
	tos->gid = pdf_font_cid_to_gid(ctx, fontdesc, cid);
	tos->fontdesc = fontdesc;

	 
	fz_expand_rect(fz_bound_glyph(ctx, fontdesc->font, tos->gid, trm, &tos->char_bbox), 1);

	return tos->gid;
}
