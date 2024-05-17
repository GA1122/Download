static void kernel_mna_trap_fault(struct pt_regs *regs, unsigned int insn)
{
	unsigned long g2 = regs->u_regs [UREG_G2];
	unsigned long fixup = search_extables_range(regs->pc, &g2);

	if (!fixup) {
		unsigned long address = compute_effective_address(regs, insn);
        	if(address < PAGE_SIZE) {
                	printk(KERN_ALERT "Unable to handle kernel NULL pointer dereference in mna handler");
        	} else
                	printk(KERN_ALERT "Unable to handle kernel paging request in mna handler");
	        printk(KERN_ALERT " at virtual address %08lx\n",address);
		printk(KERN_ALERT "current->{mm,active_mm}->context = %08lx\n",
			(current->mm ? current->mm->context :
			current->active_mm->context));
		printk(KERN_ALERT "current->{mm,active_mm}->pgd = %08lx\n",
			(current->mm ? (unsigned long) current->mm->pgd :
			(unsigned long) current->active_mm->pgd));
	        die_if_kernel("Oops", regs);
		 
	}
	regs->pc = fixup;
	regs->npc = regs->pc + 4;
	regs->u_regs [UREG_G2] = g2;
}
