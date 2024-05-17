void giveup_altivec_maybe_transactional(struct task_struct *tsk)
{
	 
	if (tsk == current && tsk->thread.regs &&
	    MSR_TM_ACTIVE(tsk->thread.regs->msr) &&
	    !test_thread_flag(TIF_RESTORE_TM)) {
		tsk->thread.tm_orig_msr = tsk->thread.regs->msr;
		set_thread_flag(TIF_RESTORE_TM);
	}

	giveup_altivec(tsk);
}
