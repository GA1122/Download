pdf_process_BMC(fz_context *ctx, pdf_processor *proc, pdf_csi *csi, const char *name)
{
	if (proc->op_BMC)
		proc->op_BMC(ctx, proc, name);
	if (proc->hidden > 0)
		++proc->hidden;
}
