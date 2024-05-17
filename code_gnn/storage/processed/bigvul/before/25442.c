int fpu_emulator_cop1Handler(struct pt_regs *xcp, struct mips_fpu_struct *ctx,
	int has_fpu, void *__user *fault_addr)
{
	unsigned long oldepc, prevepc;
	mips_instruction insn;
	int sig = 0;

	oldepc = xcp->cp0_epc;
	do {
		prevepc = xcp->cp0_epc;

		if (!access_ok(VERIFY_READ, xcp->cp0_epc, sizeof(mips_instruction))) {
			MIPS_FPU_EMU_INC_STATS(errors);
			*fault_addr = (mips_instruction __user *)xcp->cp0_epc;
			return SIGBUS;
		}
		if (__get_user(insn, (mips_instruction __user *) xcp->cp0_epc)) {
			MIPS_FPU_EMU_INC_STATS(errors);
			*fault_addr = (mips_instruction __user *)xcp->cp0_epc;
			return SIGSEGV;
		}
		if (insn == 0)
			xcp->cp0_epc += 4;	 
		else {
			 
			 
			ieee754_csr.rm = ieee_rm[ieee754_csr.rm];
			sig = cop1Emulate(xcp, ctx, fault_addr);
			 
			ieee754_csr.rm = mips_rm[ieee754_csr.rm];
		}

		if (has_fpu)
			break;
		if (sig)
			break;

		cond_resched();
	} while (xcp->cp0_epc > prevepc);

	 
	if (sig == SIGILL && xcp->cp0_epc != oldepc)
		 
		sig = 0;

	return sig;
}
