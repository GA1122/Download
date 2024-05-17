asmlinkage void do_watch(struct pt_regs *regs)
{
	u32 cause;

	 
	cause = read_c0_cause();
	cause &= ~(1 << 22);
	write_c0_cause(cause);

	 
	if (test_tsk_thread_flag(current, TIF_LOAD_WATCH)) {
		mips_read_watch_registers();
		local_irq_enable();
		force_sig(SIGTRAP, current);
	} else {
		mips_clear_watch_registers();
		local_irq_enable();
	}
}
