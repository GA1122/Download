static inline void check_stack(void)
{
	unsigned long this_size, flags;
	unsigned long *p, *top, *start;
	int i;

	this_size = ((unsigned long)&this_size) & (THREAD_SIZE-1);
	this_size = THREAD_SIZE - this_size;

	if (this_size <= max_stack_size)
		return;

	 
	if (!object_is_on_stack(&this_size))
		return;

	local_irq_save(flags);
	arch_spin_lock(&max_stack_lock);

	 
	if (this_size <= max_stack_size)
		goto out;

	max_stack_size = this_size;

	max_stack_trace.nr_entries	= 0;
	max_stack_trace.skip		= 3;

	save_stack_trace(&max_stack_trace);

	 
	i = 0;
	start = &this_size;
	top = (unsigned long *)
		(((unsigned long)start & ~(THREAD_SIZE-1)) + THREAD_SIZE);

	 
	while (i < max_stack_trace.nr_entries) {
		int found = 0;

		stack_dump_index[i] = this_size;
		p = start;

		for (; p < top && i < max_stack_trace.nr_entries; p++) {
			if (*p == stack_dump_trace[i]) {
				this_size = stack_dump_index[i++] =
					(top - p) * sizeof(unsigned long);
				found = 1;
				 
				start = p + 1;
			}
		}

		if (!found)
			i++;
	}

 out:
	arch_spin_unlock(&max_stack_lock);
	local_irq_restore(flags);
}
