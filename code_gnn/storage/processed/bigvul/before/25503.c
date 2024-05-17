void bad_page_fault(struct pt_regs *regs, unsigned long address, int sig)
{
	const struct exception_table_entry *entry;
	unsigned long *stackend;

	 
	if ((entry = search_exception_tables(regs->nip)) != NULL) {
		regs->nip = entry->fixup;
		return;
	}

	 

	switch (regs->trap) {
	case 0x300:
	case 0x380:
		printk(KERN_ALERT "Unable to handle kernel paging request for "
			"data at address 0x%08lx\n", regs->dar);
		break;
	case 0x400:
	case 0x480:
		printk(KERN_ALERT "Unable to handle kernel paging request for "
			"instruction fetch\n");
		break;
	default:
		printk(KERN_ALERT "Unable to handle kernel paging request for "
			"unknown fault\n");
		break;
	}
	printk(KERN_ALERT "Faulting instruction address: 0x%08lx\n",
		regs->nip);

	stackend = end_of_stack(current);
	if (current != &init_task && *stackend != STACK_END_MAGIC)
		printk(KERN_ALERT "Thread overran stack, or stack corrupted\n");

	die("Kernel access of bad area", regs, sig);
}
