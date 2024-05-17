static void parse_fpe(struct pt_regs *regs)
{
	int code = 0;
	unsigned long fpscr;

	flush_fp_to_thread(current);

	fpscr = current->thread.fpscr.val;

	 
	if ((fpscr & FPSCR_VE) && (fpscr & FPSCR_VX))
		code = FPE_FLTINV;

	 
	else if ((fpscr & FPSCR_OE) && (fpscr & FPSCR_OX))
		code = FPE_FLTOVF;

	 
	else if ((fpscr & FPSCR_UE) && (fpscr & FPSCR_UX))
		code = FPE_FLTUND;

	 
	else if ((fpscr & FPSCR_ZE) && (fpscr & FPSCR_ZX))
		code = FPE_FLTDIV;

	 
	else if ((fpscr & FPSCR_XE) && (fpscr & FPSCR_XX))
		code = FPE_FLTRES;

	_exception(SIGFPE, regs, code, regs->nip);
}
