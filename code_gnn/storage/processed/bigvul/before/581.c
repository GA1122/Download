pdf_process_EMC(fz_context *ctx, pdf_processor *proc, pdf_csi *csi)
{
	if (proc->op_EMC)
		proc->op_EMC(ctx, proc);
	if (proc->hidden > 0)
		--proc->hidden;
}
