static int do_insn_fetch(struct x86_emulate_ctxt *ctxt,
			 void *dest, unsigned size)
{
	int rc;

	 
	if (unlikely(ctxt->_eip + size - ctxt->eip > 15))
		return X86EMUL_UNHANDLEABLE;
	while (size--) {
		rc = do_insn_fetch_byte(ctxt, dest++);
		if (rc != X86EMUL_CONTINUE)
			return rc;
	}
	return X86EMUL_CONTINUE;
}
