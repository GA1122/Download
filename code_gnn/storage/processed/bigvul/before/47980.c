static int write_segment_descriptor(struct x86_emulate_ctxt *ctxt,
				    u16 selector, struct desc_struct *desc)
{
	int rc;
	ulong addr;

	rc = get_descriptor_ptr(ctxt, selector, &addr);
	if (rc != X86EMUL_CONTINUE)
		return rc;

	return ctxt->ops->write_std(ctxt, addr, desc, sizeof *desc,
				    &ctxt->exception);
}