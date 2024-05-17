static void perf_event_init_userpage(struct perf_event *event)
{
	struct perf_event_mmap_page *userpg;
	struct ring_buffer *rb;

	rcu_read_lock();
	rb = rcu_dereference(event->rb);
	if (!rb)
		goto unlock;

	userpg = rb->user_page;

	 
	userpg->cap_bit0_is_deprecated = 1;
	userpg->size = offsetof(struct perf_event_mmap_page, __reserved);

unlock:
	rcu_read_unlock();
}