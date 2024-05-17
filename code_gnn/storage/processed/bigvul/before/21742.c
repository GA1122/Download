static int em_cr_write(struct x86_emulate_ctxt *ctxt)
{
	if (ctxt->ops->set_cr(ctxt, ctxt->modrm_reg, ctxt->src.val))
		return emulate_gp(ctxt, 0);

	 
	ctxt->dst.type = OP_NONE;
	return X86EMUL_CONTINUE;
}