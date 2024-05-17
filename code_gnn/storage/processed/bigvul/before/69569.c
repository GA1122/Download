 static int fxrstor_fixup(struct x86_emulate_ctxt *ctxt,
		struct fxregs_state *new)
{
	int rc = X86EMUL_CONTINUE;
	struct fxregs_state old;

	rc = asm_safe("fxsave %[fx]", , [fx] "+m"(old));
	if (rc != X86EMUL_CONTINUE)
		return rc;

	 
#ifdef CONFIG_X86_64
	 
	memcpy(&new->xmm_space[8 * 16/4], &old.xmm_space[8 * 16/4], 8 * 16);
#endif

	 
	if (!(ctxt->ops->get_cr(ctxt, 4) & X86_CR4_OSFXSR))
		memcpy(new->xmm_space, old.xmm_space, 8 * 16);

	return rc;
}
