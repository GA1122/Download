static void math_error(struct pt_regs *regs, int error_code, int trapnr)
{
	struct task_struct *task = current;
	siginfo_t info;
	unsigned short err;
	char *str = (trapnr == X86_TRAP_MF) ? "fpu exception" :
						"simd exception";

	if (notify_die(DIE_TRAP, str, regs, error_code, trapnr, SIGFPE) == NOTIFY_STOP)
		return;
	conditional_sti(regs);

	if (!user_mode_vm(regs))
	{
		if (!fixup_exception(regs)) {
			task->thread.error_code = error_code;
			task->thread.trap_nr = trapnr;
			die(str, regs, error_code);
		}
		return;
	}

	 
	save_init_fpu(task);
	task->thread.trap_nr = trapnr;
	task->thread.error_code = error_code;
	info.si_signo = SIGFPE;
	info.si_errno = 0;
	info.si_addr = (void __user *)uprobe_get_trap_addr(regs);
	if (trapnr == X86_TRAP_MF) {
		unsigned short cwd, swd;
		 
		cwd = get_fpu_cwd(task);
		swd = get_fpu_swd(task);

		err = swd & ~cwd;
	} else {
		 
		unsigned short mxcsr = get_fpu_mxcsr(task);
		err = ~(mxcsr >> 7) & mxcsr;
	}

	if (err & 0x001) {	 
		 
		info.si_code = FPE_FLTINV;
	} else if (err & 0x004) {  
		info.si_code = FPE_FLTDIV;
	} else if (err & 0x008) {  
		info.si_code = FPE_FLTOVF;
	} else if (err & 0x012) {  
		info.si_code = FPE_FLTUND;
	} else if (err & 0x020) {  
		info.si_code = FPE_FLTRES;
	} else {
		 
		return;
	}
	force_sig_info(SIGFPE, &info, task);
}
