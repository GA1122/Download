pdf_process_grestore(fz_context *ctx, pdf_processor *proc, pdf_csi *csi)
{
	if (csi->gstate > 0)
	{
		if (proc->op_Q)
			proc->op_Q(ctx, proc);
		--csi->gstate;
	}
}
