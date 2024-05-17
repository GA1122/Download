__rb_reserve_next(struct ring_buffer_per_cpu *cpu_buffer,
		  struct rb_event_info *info)
{
	struct ring_buffer_event *event;
	struct buffer_page *tail_page;
	unsigned long tail, write;

	 
	if (unlikely(info->add_timestamp))
		info->length += RB_LEN_TIME_EXTEND;

	 
	tail_page = info->tail_page = READ_ONCE(cpu_buffer->tail_page);
	write = local_add_return(info->length, &tail_page->write);

	 
	write &= RB_WRITE_MASK;
	tail = write - info->length;

	 
	if (!tail)
		info->delta = 0;

	 
	if (unlikely(write > BUF_PAGE_SIZE))
		return rb_move_tail(cpu_buffer, tail, info);

	 

	event = __rb_page_index(tail_page, tail);
	kmemcheck_annotate_bitfield(event, bitfield);
	rb_update_event(cpu_buffer, event, info);

	local_inc(&tail_page->entries);

	 
	if (!tail)
		tail_page->page->time_stamp = info->ts;

	 
	local_add(info->length, &cpu_buffer->entries_bytes);

	return event;
}
