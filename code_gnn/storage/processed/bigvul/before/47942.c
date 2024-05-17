static int em_bsr_c(struct x86_emulate_ctxt *ctxt)
{
	 
	if (ctxt->src.val == 0)
		ctxt->dst.type = OP_NONE;
	return fastop(ctxt, em_bsr);
}
