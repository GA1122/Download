static inline bool owner_running(struct mutex *lock, struct task_struct *owner)
{
	bool ret = false;

	rcu_read_lock();
	if (lock->owner != owner)
		goto fail;

	 
	barrier();

	ret = owner->on_cpu;
fail:
	rcu_read_unlock();

	return ret;
}
