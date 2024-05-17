static int ptrace_setcrunchregs(struct task_struct *tsk, void __user *ufp)
{
	struct thread_info *thread = task_thread_info(tsk);

	crunch_task_release(thread);   
	return copy_from_user(&thread->crunchstate, ufp, CRUNCH_SIZE)
		? -EFAULT : 0;
}