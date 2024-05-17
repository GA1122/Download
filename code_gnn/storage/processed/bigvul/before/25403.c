static void do_trap_or_bp(struct pt_regs *regs, unsigned int code,
	const char *str)
{
	siginfo_t info;
	char b[40];

#ifdef CONFIG_KGDB_LOW_LEVEL_TRAP
	if (kgdb_ll_trap(DIE_TRAP, str, regs, code, regs_to_trapnr(regs), SIGTRAP) == NOTIFY_STOP)
		return;
#endif  

	if (notify_die(DIE_TRAP, str, regs, code, regs_to_trapnr(regs), SIGTRAP) == NOTIFY_STOP)
		return;

	 
	switch (code) {
	case BRK_OVERFLOW:
	case BRK_DIVZERO:
		scnprintf(b, sizeof(b), "%s instruction in kernel code", str);
		die_if_kernel(b, regs);
		if (code == BRK_DIVZERO)
			info.si_code = FPE_INTDIV;
		else
			info.si_code = FPE_INTOVF;
		info.si_signo = SIGFPE;
		info.si_errno = 0;
		info.si_addr = (void __user *) regs->cp0_epc;
		force_sig_info(SIGFPE, &info, current);
		break;
	case BRK_BUG:
		die_if_kernel("Kernel bug detected", regs);
		force_sig(SIGTRAP, current);
		break;
	case BRK_MEMU:
		 
		if (do_dsemulret(regs))
			return;

		die_if_kernel("Math emu break/trap", regs);
		force_sig(SIGTRAP, current);
		break;
	default:
		scnprintf(b, sizeof(b), "%s instruction in kernel code", str);
		die_if_kernel(b, regs);
		force_sig(SIGTRAP, current);
	}
}
