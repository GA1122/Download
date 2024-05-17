static void put_event(struct perf_event *event)
{
	struct task_struct *owner;

	if (!atomic_long_dec_and_test(&event->refcount))
		return;

	rcu_read_lock();
	owner = ACCESS_ONCE(event->owner);
	 
	smp_read_barrier_depends();
	if (owner) {
		 
		get_task_struct(owner);
	}
	rcu_read_unlock();

	if (owner) {
		mutex_lock(&owner->perf_event_mutex);
		 
		if (event->owner)
			list_del_init(&event->owner_entry);
		mutex_unlock(&owner->perf_event_mutex);
		put_task_struct(owner);
	}

	perf_event_release_kernel(event);
}
