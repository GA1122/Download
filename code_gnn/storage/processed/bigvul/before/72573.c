rb_reserve_next_event(struct ring_buffer *buffer,
		      struct ring_buffer_per_cpu *cpu_buffer,
		      unsigned long length)
{
	struct ring_buffer_event *event;
	struct rb_event_info info;
	int nr_loops = 0;
	u64 diff;

	rb_start_commit(cpu_buffer);

#ifdef CONFIG_RING_BUFFER_ALLOW_SWAP
	 
	barrier();
	if (unlikely(ACCESS_ONCE(cpu_buffer->buffer) != buffer)) {
		local_dec(&cpu_buffer->committing);
		local_dec(&cpu_buffer->commits);
		return NULL;
	}
#endif

	info.length = rb_calculate_event_length(length);
 again:
	info.add_timestamp = 0;
	info.delta = 0;

	 
	if (RB_WARN_ON(cpu_buffer, ++nr_loops > 1000))
		goto out_fail;

	info.ts = rb_time_stamp(cpu_buffer->buffer);
	diff = info.ts - cpu_buffer->write_stamp;

	 
	barrier();

	 
	if (likely(info.ts >= cpu_buffer->write_stamp)) {
		info.delta = diff;
		if (unlikely(test_time_stamp(info.delta)))
			rb_handle_timestamp(cpu_buffer, &info);
	}

	event = __rb_reserve_next(cpu_buffer, &info);

	if (unlikely(PTR_ERR(event) == -EAGAIN)) {
		if (info.add_timestamp)
			info.length -= RB_LEN_TIME_EXTEND;
		goto again;
	}

	if (!event)
		goto out_fail;

	return event;

 out_fail:
	rb_end_commit(cpu_buffer);
	return NULL;
}
