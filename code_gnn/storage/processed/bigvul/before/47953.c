static int em_rsm(struct x86_emulate_ctxt *ctxt)
{
	unsigned long cr0, cr4, efer;
	u64 smbase;
	int ret;

	if ((ctxt->emul_flags & X86EMUL_SMM_MASK) == 0)
		return emulate_ud(ctxt);

	 
	cr4 = ctxt->ops->get_cr(ctxt, 4);
	if (emulator_has_longmode(ctxt)) {
		struct desc_struct cs_desc;

		 
		if (cr4 & X86_CR4_PCIDE) {
			ctxt->ops->set_cr(ctxt, 4, cr4 & ~X86_CR4_PCIDE);
			cr4 &= ~X86_CR4_PCIDE;
		}

		 
		memset(&cs_desc, 0, sizeof(cs_desc));
		cs_desc.type = 0xb;
		cs_desc.s = cs_desc.g = cs_desc.p = 1;
		ctxt->ops->set_segment(ctxt, 0, &cs_desc, 0, VCPU_SREG_CS);
	}

	 
	cr0 = ctxt->ops->get_cr(ctxt, 0);
	if (cr0 & X86_CR0_PE)
		ctxt->ops->set_cr(ctxt, 0, cr0 & ~(X86_CR0_PG | X86_CR0_PE));

	 
	if (cr4 & X86_CR4_PAE)
		ctxt->ops->set_cr(ctxt, 4, cr4 & ~X86_CR4_PAE);

	 
	efer = 0;
	ctxt->ops->set_msr(ctxt, MSR_EFER, efer);

	smbase = ctxt->ops->get_smbase(ctxt);
	if (emulator_has_longmode(ctxt))
		ret = rsm_load_state_64(ctxt, smbase + 0x8000);
	else
		ret = rsm_load_state_32(ctxt, smbase + 0x8000);

	if (ret != X86EMUL_CONTINUE) {
		 
		return X86EMUL_UNHANDLEABLE;
	}

	if ((ctxt->emul_flags & X86EMUL_SMM_INSIDE_NMI_MASK) == 0)
		ctxt->ops->set_nmi_mask(ctxt, false);

	ctxt->emul_flags &= ~X86EMUL_SMM_INSIDE_NMI_MASK;
	ctxt->emul_flags &= ~X86EMUL_SMM_MASK;
	return X86EMUL_CONTINUE;
}
