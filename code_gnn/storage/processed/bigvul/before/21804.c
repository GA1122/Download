static int emulate_iret_real(struct x86_emulate_ctxt *ctxt)
{
	int rc = X86EMUL_CONTINUE;
	unsigned long temp_eip = 0;
	unsigned long temp_eflags = 0;
	unsigned long cs = 0;
	unsigned long mask = EFLG_CF | EFLG_PF | EFLG_AF | EFLG_ZF | EFLG_SF | EFLG_TF |
			     EFLG_IF | EFLG_DF | EFLG_OF | EFLG_IOPL | EFLG_NT | EFLG_RF |
			     EFLG_AC | EFLG_ID | (1 << 1);  
	unsigned long vm86_mask = EFLG_VM | EFLG_VIF | EFLG_VIP;

	 

	rc = emulate_pop(ctxt, &temp_eip, ctxt->op_bytes);

	if (rc != X86EMUL_CONTINUE)
		return rc;

	if (temp_eip & ~0xffff)
		return emulate_gp(ctxt, 0);

	rc = emulate_pop(ctxt, &cs, ctxt->op_bytes);

	if (rc != X86EMUL_CONTINUE)
		return rc;

	rc = emulate_pop(ctxt, &temp_eflags, ctxt->op_bytes);

	if (rc != X86EMUL_CONTINUE)
		return rc;

	rc = load_segment_descriptor(ctxt, (u16)cs, VCPU_SREG_CS);

	if (rc != X86EMUL_CONTINUE)
		return rc;

	ctxt->_eip = temp_eip;


	if (ctxt->op_bytes == 4)
		ctxt->eflags = ((temp_eflags & mask) | (ctxt->eflags & vm86_mask));
	else if (ctxt->op_bytes == 2) {
		ctxt->eflags &= ~0xffff;
		ctxt->eflags |= temp_eflags;
	}

	ctxt->eflags &= ~EFLG_RESERVED_ZEROS_MASK;  
	ctxt->eflags |= EFLG_RESERVED_ONE_MASK;

	return rc;
}