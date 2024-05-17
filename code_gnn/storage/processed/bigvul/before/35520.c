static int em_cmpxchg(struct x86_emulate_ctxt *ctxt)
{
	 
	ctxt->dst.orig_val = ctxt->dst.val;
	ctxt->dst.val = reg_read(ctxt, VCPU_REGS_RAX);
	ctxt->src.orig_val = ctxt->src.val;
	ctxt->src.val = ctxt->dst.orig_val;
	fastop(ctxt, em_cmp);

	if (ctxt->eflags & EFLG_ZF) {
		 
		ctxt->dst.val = ctxt->src.orig_val;
	} else {
		 
		ctxt->dst.type = OP_REG;
		ctxt->dst.addr.reg = reg_rmw(ctxt, VCPU_REGS_RAX);
		ctxt->dst.val = ctxt->dst.orig_val;
	}
	return X86EMUL_CONTINUE;
}
