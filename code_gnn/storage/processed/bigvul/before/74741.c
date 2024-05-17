void __kprobes program_check_exception(struct pt_regs *regs)
{
	unsigned int reason = get_reason(regs);
	extern int do_mathemu(struct pt_regs *regs);

#ifdef CONFIG_MATH_EMULATION
	 
	if (!(reason & REASON_TRAP) && do_mathemu(regs) == 0) {
		emulate_single_step(regs);
		return;
	}
#endif  

	if (reason & REASON_FP) {
		 
		parse_fpe(regs);
		return;
	}
	if (reason & REASON_TRAP) {
		 
		if (notify_die(DIE_BPT, "breakpoint", regs, 5, 5, SIGTRAP)
				== NOTIFY_STOP)
			return;
		if (debugger_bpt(regs))
			return;
		if (check_bug_trap(regs)) {
			regs->nip += 4;
			return;
		}
		_exception(SIGTRAP, regs, TRAP_BRKPT, regs->nip);
		return;
	}

	local_irq_enable();

	 
	if (reason & (REASON_ILLEGAL | REASON_PRIVILEGED)) {
		switch (emulate_instruction(regs)) {
		case 0:
			regs->nip += 4;
			emulate_single_step(regs);
			return;
		case -EFAULT:
			_exception(SIGSEGV, regs, SEGV_MAPERR, regs->nip);
			return;
		}
	}

	if (reason & REASON_PRIVILEGED)
		_exception(SIGILL, regs, ILL_PRVOPC, regs->nip);
	else
		_exception(SIGILL, regs, ILL_ILLOPC, regs->nip);
}
