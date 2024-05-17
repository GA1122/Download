static int ieee_fpe_handler(struct pt_regs *regs)
{
	unsigned short insn = *(unsigned short *)regs->pc;
	unsigned short finsn;
	unsigned long nextpc;
	siginfo_t info;
	int nib[4] = {
		(insn >> 12) & 0xf,
		(insn >> 8) & 0xf,
		(insn >> 4) & 0xf,
		insn & 0xf};

	if (nib[0] == 0xb ||
	    (nib[0] == 0x4 && nib[2] == 0x0 && nib[3] == 0xb))  
		regs->pr = regs->pc + 4;

	if (nib[0] == 0xa || nib[0] == 0xb) {  
		nextpc = regs->pc + 4 + ((short) ((insn & 0xfff) << 4) >> 3);
		finsn = *(unsigned short *) (regs->pc + 2);
	} else if (nib[0] == 0x8 && nib[1] == 0xd) {  
		if (regs->sr & 1)
			nextpc = regs->pc + 4 + ((char) (insn & 0xff) << 1);
		else
			nextpc = regs->pc + 4;
		finsn = *(unsigned short *) (regs->pc + 2);
	} else if (nib[0] == 0x8 && nib[1] == 0xf) {  
		if (regs->sr & 1)
			nextpc = regs->pc + 4;
		else
			nextpc = regs->pc + 4 + ((char) (insn & 0xff) << 1);
		finsn = *(unsigned short *) (regs->pc + 2);
	} else if (nib[0] == 0x4 && nib[3] == 0xb &&
		 (nib[2] == 0x0 || nib[2] == 0x2)) {  
		nextpc = regs->regs[nib[1]];
		finsn = *(unsigned short *) (regs->pc + 2);
	} else if (nib[0] == 0x0 && nib[3] == 0x3 &&
		 (nib[2] == 0x0 || nib[2] == 0x2)) {  
		nextpc = regs->pc + 4 + regs->regs[nib[1]];
		finsn = *(unsigned short *) (regs->pc + 2);
	} else if (insn == 0x000b) {  
		nextpc = regs->pr;
		finsn = *(unsigned short *) (regs->pc + 2);
	} else {
		nextpc = regs->pc + 2;
		finsn = insn;
	}

	if ((finsn & 0xf1ff) == 0xf0ad) {  
		struct task_struct *tsk = current;

		if ((tsk->thread.xstate->softfpu.fpscr & (1 << 17))) {
			 
			denormal_to_double (&tsk->thread.xstate->softfpu,
					    (finsn >> 8) & 0xf);
			tsk->thread.xstate->softfpu.fpscr &=
				~(FPSCR_CAUSE_MASK | FPSCR_FLAG_MASK);
			task_thread_info(tsk)->status |= TS_USEDFPU;
		} else {
			info.si_signo = SIGFPE;
			info.si_errno = 0;
			info.si_code = FPE_FLTINV;
			info.si_addr = (void __user *)regs->pc;
			force_sig_info(SIGFPE, &info, tsk);
		}

		regs->pc = nextpc;
		return 1;
	}

	return 0;
}