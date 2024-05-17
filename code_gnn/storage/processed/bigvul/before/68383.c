perf_event_set_output(struct perf_event *event, struct perf_event *output_event)
{
	struct ring_buffer *rb = NULL;
	int ret = -EINVAL;

	if (!output_event)
		goto set;

	 
	if (event == output_event)
		goto out;

	 
	if (output_event->cpu != event->cpu)
		goto out;

	 
	if (output_event->cpu == -1 && output_event->ctx != event->ctx)
		goto out;

	 
	if (output_event->clock != event->clock)
		goto out;

	 
	if (is_write_backward(output_event) != is_write_backward(event))
		goto out;

	 
	if (has_aux(event) && has_aux(output_event) &&
	    event->pmu != output_event->pmu)
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

	ring_buffer_attach(event, rb);

	ret = 0;
unlock:
	mutex_unlock(&event->mmap_mutex);

out:
	return ret;
}
