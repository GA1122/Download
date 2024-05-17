static ulong stack_mask(struct x86_emulate_ctxt *ctxt)
{
	u16 sel;
	struct desc_struct ss;

	if (ctxt->mode == X86EMUL_MODE_PROT64)
		return ~0UL;
	ctxt->ops->get_segment(ctxt, &sel, &ss, NULL, VCPU_SREG_SS);
	return ~0U >> ((ss.d ^ 1) * 16);   
}