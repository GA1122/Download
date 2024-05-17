static int rsm_enter_protected_mode(struct x86_emulate_ctxt *ctxt,
				     u64 cr0, u64 cr4)
{
	int bad;

	 
	bad = ctxt->ops->set_cr(ctxt, 4, cr4 & ~X86_CR4_PCIDE);
	if (bad)
		return X86EMUL_UNHANDLEABLE;

	bad = ctxt->ops->set_cr(ctxt, 0, cr0);
	if (bad)
		return X86EMUL_UNHANDLEABLE;

	if (cr4 & X86_CR4_PCIDE) {
		bad = ctxt->ops->set_cr(ctxt, 4, cr4);
		if (bad)
			return X86EMUL_UNHANDLEABLE;
	}

	return X86EMUL_CONTINUE;
}