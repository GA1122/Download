static int check_fxsr(struct x86_emulate_ctxt *ctxt)
{
	u32 eax = 1, ebx, ecx = 0, edx;

	ctxt->ops->get_cpuid(ctxt, &eax, &ebx, &ecx, &edx);
	if (!(edx & FFL(FXSR)))
		return emulate_ud(ctxt);

	if (ctxt->ops->get_cr(ctxt, 0) & (X86_CR0_TS | X86_CR0_EM))
		return emulate_nm(ctxt);

	 
	if (ctxt->mode >= X86EMUL_MODE_PROT64)
		return X86EMUL_UNHANDLEABLE;

	return X86EMUL_CONTINUE;
}