static struct task_struct *dup_task_struct(struct task_struct *orig)
{
	struct task_struct *tsk;
	struct thread_info *ti;
	unsigned long *stackend;
	int node = tsk_fork_get_node(orig);
	int err;

	tsk = alloc_task_struct_node(node);
	if (!tsk)
		return NULL;

	ti = alloc_thread_info_node(tsk, node);
	if (!ti)
		goto free_tsk;

	err = arch_dup_task_struct(tsk, orig);
	if (err)
		goto free_ti;

	tsk->stack = ti;

	setup_thread_stack(tsk, orig);
	clear_user_return_notifier(tsk);
	clear_tsk_need_resched(tsk);
	stackend = end_of_stack(tsk);
	*stackend = STACK_END_MAGIC;	 

#ifdef CONFIG_CC_STACKPROTECTOR
	tsk->stack_canary = get_random_int();
#endif

	 
	atomic_set(&tsk->usage, 2);
#ifdef CONFIG_BLK_DEV_IO_TRACE
	tsk->btrace_seq = 0;
#endif
	tsk->splice_pipe = NULL;
	tsk->task_frag.page = NULL;

	account_kernel_stack(ti, 1);

	return tsk;

free_ti:
	free_thread_info(ti);
free_tsk:
	free_task_struct(tsk);
	return NULL;
}