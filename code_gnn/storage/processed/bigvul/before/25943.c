mm_fault_error(struct pt_regs *regs, unsigned long error_code,
	       unsigned long address, unsigned int fault)
{
	 
	if (fatal_signal_pending(current)) {
		if (!(fault & VM_FAULT_RETRY))
			up_read(&current->mm->mmap_sem);
		if (!(error_code & PF_USER))
			no_context(regs, error_code, address);
		return 1;
	}
	if (!(fault & VM_FAULT_ERROR))
		return 0;

	if (fault & VM_FAULT_OOM) {
		 
		if (!(error_code & PF_USER)) {
			up_read(&current->mm->mmap_sem);
			no_context(regs, error_code, address);
			return 1;
		}

		out_of_memory(regs, error_code, address);
	} else {
		if (fault & (VM_FAULT_SIGBUS|VM_FAULT_HWPOISON|
			     VM_FAULT_HWPOISON_LARGE))
			do_sigbus(regs, error_code, address, fault);
		else
			BUG();
	}
	return 1;
}