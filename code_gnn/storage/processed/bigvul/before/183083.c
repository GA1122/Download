  static int em_jcxz(struct x86_emulate_ctxt *ctxt)
  {
// 	int rc = X86EMUL_CONTINUE;
// 
  	if (address_mask(ctxt, reg_read(ctxt, VCPU_REGS_RCX)) == 0)
		jmp_rel(ctxt, ctxt->src.val);
// 		rc = jmp_rel(ctxt, ctxt->src.val);
  
	return X86EMUL_CONTINUE;
// 	return rc;
  }