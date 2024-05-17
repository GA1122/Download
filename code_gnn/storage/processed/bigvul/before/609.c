pdf_xobject_matrix(fz_context *ctx, pdf_xobject *xobj, fz_matrix *matrix)
{
	return pdf_to_matrix(ctx, pdf_dict_get(ctx, xobj->obj, PDF_NAME_Matrix), matrix);
}
