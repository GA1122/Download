dotraplinkage void do_debug(struct pt_regs *regs, long error_code)
{
	struct task_struct *tsk = current;
	int user_icebp = 0;
	unsigned long dr6;
	int si_code;

	ist_enter(regs);

	get_debugreg(dr6, 6);
	 
	set_debugreg(0, 6);

	 
	dr6 &= ~DR6_RESERVED;

	 
	clear_tsk_thread_flag(tsk, TIF_BLOCKSTEP);

	if (unlikely(!user_mode(regs) && (dr6 & DR_STEP) &&
		     is_sysenter_singlestep(regs))) {
		dr6 &= ~DR_STEP;
		if (!dr6)
			goto exit;
		 
	}

	 
	if (!dr6 && user_mode(regs))
		user_icebp = 1;

	 
	tsk->thread.debugreg6 = dr6;

#ifdef CONFIG_KPROBES
	if (kprobe_debug_handler(regs))
		goto exit;
#endif

	if (notify_die(DIE_DEBUG, "debug", regs, (long)&dr6, error_code,
							SIGTRAP) == NOTIFY_STOP)
		goto exit;

	 
	debug_stack_usage_inc();

	 
	cond_local_irq_enable(regs);

	if (v8086_mode(regs)) {
		handle_vm86_trap((struct kernel_vm86_regs *) regs, error_code,
					X86_TRAP_DB);
		cond_local_irq_disable(regs);
		debug_stack_usage_dec();
		goto exit;
	}

	if (WARN_ON_ONCE((dr6 & DR_STEP) && !user_mode(regs))) {
		 
		tsk->thread.debugreg6 &= ~DR_STEP;
		set_tsk_thread_flag(tsk, TIF_SINGLESTEP);
		regs->flags &= ~X86_EFLAGS_TF;
	}
	si_code = get_si_code(tsk->thread.debugreg6);
	if (tsk->thread.debugreg6 & (DR_STEP | DR_TRAP_BITS) || user_icebp)
		send_sigtrap(tsk, regs, error_code, si_code);
	cond_local_irq_disable(regs);
	debug_stack_usage_dec();

exit:
	ist_exit(regs);
}
