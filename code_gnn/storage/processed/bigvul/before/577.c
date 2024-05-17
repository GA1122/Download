pdf_process_BDC(fz_context *ctx, pdf_processor *proc, pdf_csi *csi)
{
	if (proc->op_BDC)
		proc->op_BDC(ctx, proc, csi->name, csi->obj, resolve_properties(ctx, csi, csi->obj));

	 
	if (proc->hidden > 0)
	{
		++proc->hidden;
		return;
	}

	 
	if (strcmp(csi->name, "OC"))
		return;

	if (pdf_is_hidden_ocg(ctx, csi->doc->ocg, csi->rdb, proc->usage, csi->obj))
		++proc->hidden;
}
