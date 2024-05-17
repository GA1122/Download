static void kernel_mna_trap_fault(int fixup_tstate_asi)
{
	struct pt_regs *regs = current_thread_info()->kern_una_regs;
	unsigned int insn = current_thread_info()->kern_una_insn;
	const struct exception_table_entry *entry;

	entry = search_exception_tables(regs->tpc);
	if (!entry) {
		unsigned long address;

		address = compute_effective_address(regs, insn,
						    ((insn >> 25) & 0x1f));
        	if (address < PAGE_SIZE) {
                	printk(KERN_ALERT "Unable to handle kernel NULL "
			       "pointer dereference in mna handler");
        	} else
                	printk(KERN_ALERT "Unable to handle kernel paging "
			       "request in mna handler");
	        printk(KERN_ALERT " at virtual address %016lx\n",address);
		printk(KERN_ALERT "current->{active_,}mm->context = %016lx\n",
			(current->mm ? CTX_HWBITS(current->mm->context) :
			CTX_HWBITS(current->active_mm->context)));
		printk(KERN_ALERT "current->{active_,}mm->pgd = %016lx\n",
			(current->mm ? (unsigned long) current->mm->pgd :
			(unsigned long) current->active_mm->pgd));
	        die_if_kernel("Oops", regs);
		 
	}
	regs->tpc = entry->fixup;
	regs->tnpc = regs->tpc + 4;

	if (fixup_tstate_asi) {
		regs->tstate &= ~TSTATE_ASI;
		regs->tstate |= (ASI_AIUS << 24UL);
	}
}