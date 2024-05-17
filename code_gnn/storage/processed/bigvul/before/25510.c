void __kprobes do_dat_exception(struct pt_regs *regs, long pgm_int_code,
				unsigned long trans_exc_code)
{
	int access, fault;

	access = VM_READ | VM_EXEC | VM_WRITE;
	fault = do_exception(regs, access, trans_exc_code);
	if (unlikely(fault))
		do_fault_error(regs, pgm_int_code & 255, trans_exc_code, fault);
}
