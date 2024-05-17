static noinline void do_no_context(struct pt_regs *regs, long int_code,
				   unsigned long trans_exc_code)
{
	const struct exception_table_entry *fixup;
	unsigned long address;

	 
	fixup = search_exception_tables(regs->psw.addr & PSW_ADDR_INSN);
	if (fixup) {
		regs->psw.addr = fixup->fixup | PSW_ADDR_AMODE;
		return;
	}

	 
	address = trans_exc_code & __FAIL_ADDR_MASK;
	if (!user_space_fault(trans_exc_code))
		printk(KERN_ALERT "Unable to handle kernel pointer dereference"
		       " at virtual kernel address %p\n", (void *)address);
	else
		printk(KERN_ALERT "Unable to handle kernel paging request"
		       " at virtual user address %p\n", (void *)address);

	die("Oops", regs, int_code);
	do_exit(SIGKILL);
}
