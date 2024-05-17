static int ptrace_getwmmxregs(struct task_struct *tsk, void __user *ufp)
{
	struct thread_info *thread = task_thread_info(tsk);

	if (!test_ti_thread_flag(thread, TIF_USING_IWMMXT))
		return -ENODATA;
	iwmmxt_task_disable(thread);   
	return copy_to_user(ufp, &thread->fpstate.iwmmxt, IWMMXT_SIZE)
		? -EFAULT : 0;
}