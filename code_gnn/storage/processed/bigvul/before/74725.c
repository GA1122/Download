static int check_bug_trap(struct pt_regs *regs)
{
	struct bug_entry *bug;
	unsigned long addr;

	if (regs->msr & MSR_PR)
		return 0;	 
	addr = regs->nip;	 
	if (addr < PAGE_OFFSET)
		return 0;
	bug = find_bug(regs->nip);
	if (bug == NULL)
		return 0;
	if (bug->line & BUG_WARNING_TRAP) {
		 
		printk(KERN_ERR "Badness in %s at %s:%ld\n",
		       bug->function, bug->file,
		       bug->line & ~BUG_WARNING_TRAP);
		dump_stack();
		return 1;
	}
	printk(KERN_CRIT "kernel BUG in %s at %s:%ld!\n",
	       bug->function, bug->file, bug->line);

	return 0;
}
