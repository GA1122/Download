static int em_out(struct x86_emulate_ctxt *ctxt)
{
	ctxt->ops->pio_out_emulated(ctxt, ctxt->src.bytes, ctxt->dst.val,
				    &ctxt->src.val, 1);
	 
	ctxt->dst.type = OP_NONE;
	return X86EMUL_CONTINUE;
}