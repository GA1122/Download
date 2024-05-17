int mutex_spin_on_owner(struct mutex *lock, struct task_struct *owner)
{
	if (!sched_feat(OWNER_SPIN))
		return 0;

	while (owner_running(lock, owner)) {
		if (need_resched())
			return 0;

		arch_mutex_cpu_relax();
	}

	 
	if (lock->owner)
		return 0;

	return 1;
}
