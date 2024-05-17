static int em_vmcall(struct x86_emulate_ctxt *ctxt)
{
	int rc = ctxt->ops->fix_hypercall(ctxt);

	if (rc != X86EMUL_CONTINUE)
		return rc;

	 
	ctxt->_eip = ctxt->eip;
	 
	ctxt->dst.type = OP_NONE;
	return X86EMUL_CONTINUE;
}
