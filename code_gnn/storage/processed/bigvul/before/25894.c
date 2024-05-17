static unsigned long get_flags(struct task_struct *task)
{
	unsigned long retval = task_pt_regs(task)->flags;

	 
	if (test_tsk_thread_flag(task, TIF_FORCED_TF))
		retval &= ~X86_EFLAGS_TF;

	return retval;
}