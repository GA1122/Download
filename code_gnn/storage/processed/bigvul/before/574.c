pdf_drop_processor(fz_context *ctx, pdf_processor *proc)
{
	if (proc)
	{
		if (proc->close_processor)
			fz_warn(ctx, "dropping unclosed PDF processor");
		if (proc->drop_processor)
			proc->drop_processor(ctx, proc);
	}
	fz_free(ctx, proc);
}
