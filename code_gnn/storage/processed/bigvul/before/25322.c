static int ptrace_getcrunchregs(struct task_struct *tsk, void __user *ufp)
{
	struct thread_info *thread = task_thread_info(tsk);

	crunch_task_disable(thread);   
	return copy_to_user(ufp, &thread->crunchstate, CRUNCH_SIZE)
		? -EFAULT : 0;
}