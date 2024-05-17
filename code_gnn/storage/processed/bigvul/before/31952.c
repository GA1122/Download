perf_event_set_output(struct perf_event *event, struct perf_event *output_event)
{
	struct ring_buffer *rb = NULL, *old_rb = NULL;
	int ret = -EINVAL;

	if (!output_event)
		goto set;

	 
	if (event == output_event)
		goto out;

	 
	if (output_event->cpu != event->cpu)
		goto out;

	 
	if (output_event->cpu == -1 && output_event->ctx != event->ctx)
		goto out;

set:
	mutex_lock(&event->mmap_mutex);
	 
	if (atomic_read(&event->mmap_count))
		goto unlock;

	if (output_event) {
		 
		rb = ring_buffer_get(output_event);
		if (!rb)
			goto unlock;
	}

	old_rb = event->rb;
	rcu_assign_pointer(event->rb, rb);
	if (old_rb)
		ring_buffer_detach(event, old_rb);
	ret = 0;
unlock:
	mutex_unlock(&event->mmap_mutex);

	if (old_rb)
		ring_buffer_put(old_rb);
out:
	return ret;
}
