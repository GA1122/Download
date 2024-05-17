static unsigned int perf_poll(struct file *file, poll_table *wait)
{
	struct perf_event *event = file->private_data;
	struct ring_buffer *rb;
	unsigned int events = POLL_HUP;

	 
	mutex_lock(&event->mmap_mutex);

	rcu_read_lock();
	rb = rcu_dereference(event->rb);
	if (rb) {
		ring_buffer_attach(event, rb);
		events = atomic_xchg(&rb->poll, 0);
	}
	rcu_read_unlock();

	mutex_unlock(&event->mmap_mutex);

	poll_wait(file, &event->waitq, wait);

	return events;
}
