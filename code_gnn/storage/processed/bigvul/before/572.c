pdf_clear_stack(fz_context *ctx, pdf_csi *csi)
{
	int i;

	pdf_drop_obj(ctx, csi->obj);
	csi->obj = NULL;

	csi->name[0] = 0;
	csi->string_len = 0;
	for (i = 0; i < csi->top; i++)
		csi->stack[i] = 0;

	csi->top = 0;
}
