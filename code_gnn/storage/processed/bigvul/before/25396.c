asmlinkage void do_fpe(struct pt_regs *regs, unsigned long fcr31)
{
	siginfo_t info = {0};

	if (notify_die(DIE_FP, "FP exception", regs, 0, regs_to_trapnr(regs), SIGFPE)
	    == NOTIFY_STOP)
		return;
	die_if_kernel("FP exception in kernel code", regs);

	if (fcr31 & FPU_CSR_UNI_X) {
		int sig;
		void __user *fault_addr = NULL;

		 
		 
		lose_fpu(1);

		 
		sig = fpu_emulator_cop1Handler(regs, &current->thread.fpu, 1,
					       &fault_addr);

		 
		current->thread.fpu.fcr31 &= ~FPU_CSR_ALL_X;

		 
		own_fpu(1);	 

		 
		process_fpemu_return(sig, fault_addr);

		return;
	} else if (fcr31 & FPU_CSR_INV_X)
		info.si_code = FPE_FLTINV;
	else if (fcr31 & FPU_CSR_DIV_X)
		info.si_code = FPE_FLTDIV;
	else if (fcr31 & FPU_CSR_OVF_X)
		info.si_code = FPE_FLTOVF;
	else if (fcr31 & FPU_CSR_UDF_X)
		info.si_code = FPE_FLTUND;
	else if (fcr31 & FPU_CSR_INE_X)
		info.si_code = FPE_FLTRES;
	else
		info.si_code = __SI_FAULT;
	info.si_signo = SIGFPE;
	info.si_errno = 0;
	info.si_addr = (void __user *) regs->cp0_epc;
	force_sig_info(SIGFPE, &info, current);
}
