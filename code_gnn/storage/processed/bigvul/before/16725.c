pdf_obj *pdf_trailer(fz_context *ctx, pdf_document *doc)
{
	 
	pdf_xref *xref = &doc->xref_sections[0];

	return xref ? xref->trailer : NULL;
}
