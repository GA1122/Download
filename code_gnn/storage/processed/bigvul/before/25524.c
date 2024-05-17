static inline void report_user_fault(struct pt_regs *regs, long int_code,
				     int signr, unsigned long address)
{
	if ((task_pid_nr(current) > 1) && !show_unhandled_signals)
		return;
	if (!unhandled_signal(current, signr))
		return;
	if (!printk_ratelimit())
		return;
	printk("User process fault: interruption code 0x%lX ", int_code);
	print_vma_addr(KERN_CONT "in ", regs->psw.addr & PSW_ADDR_INSN);
	printk("\n");
	printk("failing address: %lX\n", address);
	show_regs(regs);
}
