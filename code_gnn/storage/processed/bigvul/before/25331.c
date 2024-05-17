static int ptrace_setwmmxregs(struct task_struct *tsk, void __user *ufp)
{
	struct thread_info *thread = task_thread_info(tsk);

	if (!test_ti_thread_flag(thread, TIF_USING_IWMMXT))
		return -EACCES;
	iwmmxt_task_release(thread);   
	return copy_from_user(&thread->fpstate.iwmmxt, ufp, IWMMXT_SIZE)
		? -EFAULT : 0;
}