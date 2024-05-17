pdf_process_gsave(fz_context *ctx, pdf_processor *proc, pdf_csi *csi)
{
	if (proc->op_q)
		proc->op_q(ctx, proc);
	++csi->gstate;
}
