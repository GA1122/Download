void __noreturn die(const char *str, struct pt_regs *regs)
{
	static int die_counter;
	int sig = SIGSEGV;
#ifdef CONFIG_MIPS_MT_SMTC
	unsigned long dvpret = dvpe();
#endif  

	if (notify_die(DIE_OOPS, str, regs, 0, regs_to_trapnr(regs), SIGSEGV) == NOTIFY_STOP)
		sig = 0;

	console_verbose();
	spin_lock_irq(&die_lock);
	bust_spinlocks(1);
#ifdef CONFIG_MIPS_MT_SMTC
	mips_mt_regdump(dvpret);
#endif  

	printk("%s[#%d]:\n", str, ++die_counter);
	show_registers(regs);
	add_taint(TAINT_DIE);
	spin_unlock_irq(&die_lock);

	if (in_interrupt())
		panic("Fatal exception in interrupt");

	if (panic_on_oops) {
		printk(KERN_EMERG "Fatal exception: panic in 5 seconds\n");
		ssleep(5);
		panic("Fatal exception");
	}

	do_exit(sig);
}
