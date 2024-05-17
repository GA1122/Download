static int perf_release(struct inode *inode, struct file *file)
{
	struct perf_event *event = file->private_data;
	struct task_struct *owner;

	file->private_data = NULL;

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

	return perf_event_release_kernel(event);
}
