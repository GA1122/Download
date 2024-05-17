static unsigned int perf_poll(struct file *file, poll_table *wait)
{
	struct perf_event *event = file->private_data;
	struct ring_buffer *rb;
	unsigned int events = POLL_HUP;

	rcu_read_lock();
	rb = rcu_dereference(event->rb);
	if (rb)
		events = atomic_xchg(&rb->poll, 0);
	rcu_read_unlock();

	poll_wait(file, &event->waitq, wait);

	return events;
}
