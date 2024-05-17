static inline void tm_reclaim_task(struct task_struct *tsk)
{
	 
	struct thread_struct *thr = &tsk->thread;

	if (!thr->regs)
		return;

	if (!MSR_TM_ACTIVE(thr->regs->msr))
		goto out_and_saveregs;

	 
	if (!test_ti_thread_flag(task_thread_info(tsk), TIF_RESTORE_TM))
		thr->tm_orig_msr = thr->regs->msr;

	TM_DEBUG("--- tm_reclaim on pid %d (NIP=%lx, "
		 "ccr=%lx, msr=%lx, trap=%lx)\n",
		 tsk->pid, thr->regs->nip,
		 thr->regs->ccr, thr->regs->msr,
		 thr->regs->trap);

	tm_reclaim_thread(thr, task_thread_info(tsk), TM_CAUSE_RESCHED);

	TM_DEBUG("--- tm_reclaim on pid %d complete\n",
		 tsk->pid);

out_and_saveregs:
	 
	tm_save_sprs(thr);
}
