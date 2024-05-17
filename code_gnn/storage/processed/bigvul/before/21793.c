static int em_vmcall(struct x86_emulate_ctxt *ctxt)
{
	int rc;

	if (ctxt->modrm_mod != 3 || ctxt->modrm_rm != 1)
		return X86EMUL_UNHANDLEABLE;

	rc = ctxt->ops->fix_hypercall(ctxt);
	if (rc != X86EMUL_CONTINUE)
		return rc;

	 
	ctxt->_eip = ctxt->eip;
	 
	ctxt->dst.type = OP_NONE;
	return X86EMUL_CONTINUE;
}