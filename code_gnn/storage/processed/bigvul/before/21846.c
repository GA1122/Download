static bool string_insn_completed(struct x86_emulate_ctxt *ctxt)
{
	 
	if (((ctxt->b == 0xa6) || (ctxt->b == 0xa7) ||
	     (ctxt->b == 0xae) || (ctxt->b == 0xaf))
	    && (((ctxt->rep_prefix == REPE_PREFIX) &&
		 ((ctxt->eflags & EFLG_ZF) == 0))
		|| ((ctxt->rep_prefix == REPNE_PREFIX) &&
		    ((ctxt->eflags & EFLG_ZF) == EFLG_ZF))))
		return true;

	return false;
}