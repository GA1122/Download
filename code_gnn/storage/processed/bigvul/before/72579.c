static void rb_tail_page_update(struct ring_buffer_per_cpu *cpu_buffer,
			       struct buffer_page *tail_page,
			       struct buffer_page *next_page)
{
	unsigned long old_entries;
	unsigned long old_write;

	 
	old_write = local_add_return(RB_WRITE_INTCNT, &next_page->write);
	old_entries = local_add_return(RB_WRITE_INTCNT, &next_page->entries);

	 
	barrier();

	 
	if (tail_page == READ_ONCE(cpu_buffer->tail_page)) {
		 
		unsigned long val = old_write & ~RB_WRITE_MASK;
		unsigned long eval = old_entries & ~RB_WRITE_MASK;

		 
		(void)local_cmpxchg(&next_page->write, old_write, val);
		(void)local_cmpxchg(&next_page->entries, old_entries, eval);

		 
		local_set(&next_page->page->commit, 0);

		 
		(void)cmpxchg(&cpu_buffer->tail_page, tail_page, next_page);
	}
}
