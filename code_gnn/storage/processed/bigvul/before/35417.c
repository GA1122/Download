do_spurious_interrupt_bug(struct pt_regs *regs, long error_code)
{
	conditional_sti(regs);
#if 0
	 
	pr_info("Ignoring P6 Local APIC Spurious Interrupt Bug...\n");
#endif
}
