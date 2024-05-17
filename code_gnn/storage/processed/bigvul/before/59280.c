void free_task(struct task_struct *tsk)
{
#ifndef CONFIG_THREAD_INFO_IN_TASK
	 
	release_task_stack(tsk);
#else
	 
	WARN_ON_ONCE(atomic_read(&tsk->stack_refcount) != 0);
#endif
	rt_mutex_debug_task_free(tsk);
	ftrace_graph_exit_task(tsk);
	put_seccomp_filter(tsk);
	arch_release_task_struct(tsk);
	if (tsk->flags & PF_KTHREAD)
		free_kthread_struct(tsk);
	free_task_struct(tsk);
}
