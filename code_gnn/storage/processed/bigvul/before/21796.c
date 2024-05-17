static int em_xchg(struct x86_emulate_ctxt *ctxt)
{
	 
	ctxt->src.val = ctxt->dst.val;
	write_register_operand(&ctxt->src);

	 
	ctxt->dst.val = ctxt->src.orig_val;
	ctxt->lock_prefix = 1;
	return X86EMUL_CONTINUE;
}
