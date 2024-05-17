static void oops_end(unsigned long flags, struct pt_regs *regs, int signr)
{
	if (regs && kexec_should_crash(current))
		crash_kexec(regs);

	bust_spinlocks(0);
	die_owner = -1;
	add_taint(TAINT_DIE, LOCKDEP_NOW_UNRELIABLE);
	die_nest_count--;
	if (!die_nest_count)
		 
		arch_spin_unlock(&die_lock);
	raw_local_irq_restore(flags);
	oops_exit();

	if (in_interrupt())
		panic("Fatal exception in interrupt");
	if (panic_on_oops)
		panic("Fatal exception");
	if (signr)
		do_exit(signr);
}