static int em_wrmsr(struct x86_emulate_ctxt *ctxt)
{
	u64 msr_data;

	msr_data = (u32)reg_read(ctxt, VCPU_REGS_RAX)
		| ((u64)reg_read(ctxt, VCPU_REGS_RDX) << 32);
	if (ctxt->ops->set_msr(ctxt, reg_read(ctxt, VCPU_REGS_RCX), msr_data))
		return emulate_gp(ctxt, 0);

	return X86EMUL_CONTINUE;
}