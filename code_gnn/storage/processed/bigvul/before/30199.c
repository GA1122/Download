static int ftrace_process_locs(struct module *mod,
			       unsigned long *start,
			       unsigned long *end)
{
	struct ftrace_page *start_pg;
	struct ftrace_page *pg;
	struct dyn_ftrace *rec;
	unsigned long count;
	unsigned long *p;
	unsigned long addr;
	unsigned long flags = 0;  
	int ret = -ENOMEM;

	count = end - start;

	if (!count)
		return 0;

	sort(start, count, sizeof(*start),
	     ftrace_cmp_ips, ftrace_swap_ips);

	start_pg = ftrace_allocate_pages(count);
	if (!start_pg)
		return -ENOMEM;

	mutex_lock(&ftrace_lock);

	 
	if (!mod) {
		WARN_ON(ftrace_pages || ftrace_pages_start);
		 
		ftrace_pages = ftrace_pages_start = start_pg;
	} else {
		if (!ftrace_pages)
			goto out;

		if (WARN_ON(ftrace_pages->next)) {
			 
			while (ftrace_pages->next)
				ftrace_pages = ftrace_pages->next;
		}

		ftrace_pages->next = start_pg;
	}

	p = start;
	pg = start_pg;
	while (p < end) {
		addr = ftrace_call_adjust(*p++);
		 
		if (!addr)
			continue;

		if (pg->index == pg->size) {
			 
			if (WARN_ON(!pg->next))
				break;
			pg = pg->next;
		}

		rec = &pg->records[pg->index++];
		rec->ip = addr;
	}

	 
	WARN_ON(pg->next);

	 
	ftrace_pages = pg;

	 
	ftrace_new_pgs = start_pg;

	 
	if (!mod)
		local_irq_save(flags);
	ftrace_update_code(mod);
	if (!mod)
		local_irq_restore(flags);
	ret = 0;
 out:
	mutex_unlock(&ftrace_lock);

	return ret;
}
