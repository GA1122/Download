static int em_cmpxchg(struct x86_emulate_ctxt *ctxt)
{
	 
	ctxt->src.orig_val = ctxt->src.val;
	ctxt->src.val = ctxt->regs[VCPU_REGS_RAX];
	emulate_2op_SrcV(ctxt, "cmp");

	if (ctxt->eflags & EFLG_ZF) {
		 
		ctxt->dst.val = ctxt->src.orig_val;
	} else {
		 
		ctxt->dst.type = OP_REG;
		ctxt->dst.addr.reg = (unsigned long *)&ctxt->regs[VCPU_REGS_RAX];
	}
	return X86EMUL_CONTINUE;
}
