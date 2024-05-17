static void perf_event_addr_filters_apply(struct perf_event *event)
{
	struct perf_addr_filters_head *ifh = perf_event_addr_filters(event);
	struct task_struct *task = READ_ONCE(event->ctx->task);
	struct perf_addr_filter *filter;
	struct mm_struct *mm = NULL;
	unsigned int count = 0;
	unsigned long flags;

	 
	if (task == TASK_TOMBSTONE)
		return;

	if (!ifh->nr_file_filters)
		return;

	mm = get_task_mm(event->ctx->task);
	if (!mm)
		goto restart;

	down_read(&mm->mmap_sem);

	raw_spin_lock_irqsave(&ifh->lock, flags);
	list_for_each_entry(filter, &ifh->list, entry) {
		event->addr_filters_offs[count] = 0;

		 
		if (filter->inode)
			event->addr_filters_offs[count] =
				perf_addr_filter_apply(filter, mm);

		count++;
	}

	event->addr_filters_gen++;
	raw_spin_unlock_irqrestore(&ifh->lock, flags);

	up_read(&mm->mmap_sem);

	mmput(mm);

restart:
	perf_event_stop(event, 1);
}
