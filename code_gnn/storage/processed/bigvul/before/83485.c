dotraplinkage void do_bounds(struct pt_regs *regs, long error_code)
{
	const struct mpx_bndcsr *bndcsr;
	siginfo_t *info;

	RCU_LOCKDEP_WARN(!rcu_is_watching(), "entry code didn't wake RCU");
	if (notify_die(DIE_TRAP, "bounds", regs, error_code,
			X86_TRAP_BR, SIGSEGV) == NOTIFY_STOP)
		return;
	cond_local_irq_enable(regs);

	if (!user_mode(regs))
		die("bounds", regs, error_code);

	if (!cpu_feature_enabled(X86_FEATURE_MPX)) {
		 
		goto exit_trap;
	}

	 
	bndcsr = get_xsave_field_ptr(XFEATURE_MASK_BNDCSR);
	if (!bndcsr)
		goto exit_trap;

	trace_bounds_exception_mpx(bndcsr);
	 
	switch (bndcsr->bndstatus & MPX_BNDSTA_ERROR_CODE) {
	case 2:	 
		if (mpx_handle_bd_fault())
			goto exit_trap;
		break;  
	case 1:  
		info = mpx_generate_siginfo(regs);
		if (IS_ERR(info)) {
			 
			goto exit_trap;
		}
		 
		do_trap(X86_TRAP_BR, SIGSEGV, "bounds", regs, error_code, info);
		kfree(info);
		break;
	case 0:  
		goto exit_trap;
	default:
		die("bounds", regs, error_code);
	}

	return;

exit_trap:
	 
	do_trap(X86_TRAP_BR, SIGSEGV, "bounds", regs, error_code, NULL);
}
