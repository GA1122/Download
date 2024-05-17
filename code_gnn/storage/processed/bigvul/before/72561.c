rb_move_tail(struct ring_buffer_per_cpu *cpu_buffer,
	     unsigned long tail, struct rb_event_info *info)
{
	struct buffer_page *tail_page = info->tail_page;
	struct buffer_page *commit_page = cpu_buffer->commit_page;
	struct ring_buffer *buffer = cpu_buffer->buffer;
	struct buffer_page *next_page;
	int ret;

	next_page = tail_page;

	rb_inc_page(cpu_buffer, &next_page);

	 
	if (unlikely(next_page == commit_page)) {
		local_inc(&cpu_buffer->commit_overrun);
		goto out_reset;
	}

	 
	if (rb_is_head_page(cpu_buffer, next_page, &tail_page->list)) {

		 
		if (!rb_is_reader_page(cpu_buffer->commit_page)) {
			 
			if (!(buffer->flags & RB_FL_OVERWRITE)) {
				local_inc(&cpu_buffer->dropped_events);
				goto out_reset;
			}

			ret = rb_handle_head_page(cpu_buffer,
						  tail_page,
						  next_page);
			if (ret < 0)
				goto out_reset;
			if (ret)
				goto out_again;
		} else {
			 
			if (unlikely((cpu_buffer->commit_page !=
				      cpu_buffer->tail_page) &&
				     (cpu_buffer->commit_page ==
				      cpu_buffer->reader_page))) {
				local_inc(&cpu_buffer->commit_overrun);
				goto out_reset;
			}
		}
	}

	rb_tail_page_update(cpu_buffer, tail_page, next_page);

 out_again:

	rb_reset_tail(cpu_buffer, tail, info);

	 
	rb_end_commit(cpu_buffer);
	 
	local_inc(&cpu_buffer->committing);

	 
	return ERR_PTR(-EAGAIN);

 out_reset:
	 
	rb_reset_tail(cpu_buffer, tail, info);

	return NULL;
}
