static noinline void do_fault_error(struct pt_regs *regs, long int_code,
				    unsigned long trans_exc_code, int fault)
{
	int si_code;

	switch (fault) {
	case VM_FAULT_BADACCESS:
	case VM_FAULT_BADMAP:
		 
		if (regs->psw.mask & PSW_MASK_PSTATE) {
			 
			si_code = (fault == VM_FAULT_BADMAP) ?
				SEGV_MAPERR : SEGV_ACCERR;
			do_sigsegv(regs, int_code, si_code, trans_exc_code);
			return;
		}
	case VM_FAULT_BADCONTEXT:
		do_no_context(regs, int_code, trans_exc_code);
		break;
	default:  
		if (fault & VM_FAULT_OOM) {
			if (!(regs->psw.mask & PSW_MASK_PSTATE))
				do_no_context(regs, int_code, trans_exc_code);
			else
				pagefault_out_of_memory();
		} else if (fault & VM_FAULT_SIGBUS) {
			 
			if (!(regs->psw.mask & PSW_MASK_PSTATE))
				do_no_context(regs, int_code, trans_exc_code);
			else
				do_sigbus(regs, int_code, trans_exc_code);
		} else
			BUG();
		break;
	}
}
