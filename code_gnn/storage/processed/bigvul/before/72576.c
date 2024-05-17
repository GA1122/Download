rb_set_commit_to_write(struct ring_buffer_per_cpu *cpu_buffer)
{
	unsigned long max_count;

	 
 again:
	max_count = cpu_buffer->nr_pages * 100;

	while (cpu_buffer->commit_page != READ_ONCE(cpu_buffer->tail_page)) {
		if (RB_WARN_ON(cpu_buffer, !(--max_count)))
			return;
		if (RB_WARN_ON(cpu_buffer,
			       rb_is_reader_page(cpu_buffer->tail_page)))
			return;
		local_set(&cpu_buffer->commit_page->page->commit,
			  rb_page_write(cpu_buffer->commit_page));
		rb_inc_page(cpu_buffer, &cpu_buffer->commit_page);
		 
		if (rb_page_write(cpu_buffer->commit_page))
			cpu_buffer->write_stamp =
				cpu_buffer->commit_page->page->time_stamp;
		 
		barrier();
	}
	while (rb_commit_index(cpu_buffer) !=
	       rb_page_write(cpu_buffer->commit_page)) {

		local_set(&cpu_buffer->commit_page->page->commit,
			  rb_page_write(cpu_buffer->commit_page));
		RB_WARN_ON(cpu_buffer,
			   local_read(&cpu_buffer->commit_page->page->commit) &
			   ~RB_WRITE_MASK);
		barrier();
	}

	 
	barrier();

	 
	if (unlikely(cpu_buffer->commit_page != READ_ONCE(cpu_buffer->tail_page)))
		goto again;
}
