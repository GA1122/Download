pdf_process_end(fz_context *ctx, pdf_processor *proc, pdf_csi *csi)
{
	while (csi->gstate > 0)
		pdf_process_grestore(ctx, proc, csi);
	if (proc->op_END)
		proc->op_END(ctx, proc);
}
