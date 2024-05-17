void alignment_exception(struct pt_regs *regs)
{
	int fixed = 0;

	 
	if (!(current->thread.align_ctl & PR_UNALIGN_SIGBUS))
		fixed = fix_alignment(regs);

	if (fixed == 1) {
		regs->nip += 4;	 
		emulate_single_step(regs);
		return;
	}

	 
	if (fixed == -EFAULT) {
		if (user_mode(regs))
			_exception(SIGSEGV, regs, SEGV_ACCERR, regs->dar);
		else
			 
			bad_page_fault(regs, regs->dar, SIGSEGV);
		return;
	}
	_exception(SIGBUS, regs, BUS_ADRALN, regs->dar);
}
