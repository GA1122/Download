static void perf_remove_from_owner(struct perf_event *event)
{
	struct task_struct *owner;

	rcu_read_lock();
	 
	owner = lockless_dereference(event->owner);
	if (owner) {
		 
		get_task_struct(owner);
	}
	rcu_read_unlock();

	if (owner) {
		 
		mutex_lock_nested(&owner->perf_event_mutex, SINGLE_DEPTH_NESTING);

		 
		if (event->owner) {
			list_del_init(&event->owner_entry);
			smp_store_release(&event->owner, NULL);
		}
		mutex_unlock(&owner->perf_event_mutex);
		put_task_struct(owner);
	}
}
