void __kprobes do_protection_exception(struct pt_regs *regs, long pgm_int_code,
				       unsigned long trans_exc_code)
{
	int fault;

	 
	regs->psw.addr -= (pgm_int_code >> 16);
	 
	if (unlikely(!(trans_exc_code & 4))) {
		do_low_address(regs, pgm_int_code, trans_exc_code);
		return;
	}
	fault = do_exception(regs, VM_WRITE, trans_exc_code);
	if (unlikely(fault))
		do_fault_error(regs, 4, trans_exc_code, fault);
}
