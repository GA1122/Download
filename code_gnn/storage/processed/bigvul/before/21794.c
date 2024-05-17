static int em_vmmcall(struct x86_emulate_ctxt *ctxt)
{
	int rc;

	rc = ctxt->ops->fix_hypercall(ctxt);

	 
	ctxt->dst.type = OP_NONE;
	return rc;
}
