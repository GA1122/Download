void rcuwait_wake_up(struct rcuwait *w)
{
	struct task_struct *task;

	rcu_read_lock();

	 
	smp_rmb();  

	 
	task = rcu_dereference(w->task);
	if (task)
		wake_up_process(task);
	rcu_read_unlock();
}
