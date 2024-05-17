void flush_fp_to_thread(struct task_struct *tsk)
{
	if (tsk->thread.regs) {
		 
		preempt_disable();
		if (tsk->thread.regs->msr & MSR_FP) {
#ifdef CONFIG_SMP
			 
			BUG_ON(tsk != current);
#endif
			giveup_fpu_maybe_transactional(tsk);
		}
		preempt_enable();
	}
}
