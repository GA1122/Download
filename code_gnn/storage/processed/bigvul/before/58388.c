static void __do_kernel_fault(struct mm_struct *mm, unsigned long addr,
			      unsigned int esr, struct pt_regs *regs)
{
	 
	if (fixup_exception(regs))
		return;

	 
	bust_spinlocks(1);
	pr_alert("Unable to handle kernel %s at virtual address %08lx\n",
		 (addr < PAGE_SIZE) ? "NULL pointer dereference" :
		 "paging request", addr);

	show_pte(mm, addr);
	die("Oops", regs, esr);
	bust_spinlocks(0);
	do_exit(SIGKILL);
}
