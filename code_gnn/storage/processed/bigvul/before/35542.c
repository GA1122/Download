static int em_push(struct x86_emulate_ctxt *ctxt)
{
	 
	ctxt->dst.type = OP_NONE;
	return push(ctxt, &ctxt->src.val, ctxt->op_bytes);
}
