struct task_struct *task_rcu_dereference(struct task_struct **ptask)
{
	struct sighand_struct *sighand;
	struct task_struct *task;

	 
retry:
	task = rcu_dereference(*ptask);
	if (!task)
		return NULL;

	probe_kernel_address(&task->sighand, sighand);

	 
	smp_rmb();
	if (unlikely(task != READ_ONCE(*ptask)))
		goto retry;

	 
	if (!sighand)
		return NULL;

	return task;
}
