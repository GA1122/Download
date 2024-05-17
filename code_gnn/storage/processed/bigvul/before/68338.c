static void perf_addr_filters_adjust(struct vm_area_struct *vma)
{
	struct perf_event_context *ctx;
	int ctxn;

	 
	if (!(vma->vm_flags & VM_EXEC))
		return;

	rcu_read_lock();
	for_each_task_context_nr(ctxn) {
		ctx = rcu_dereference(current->perf_event_ctxp[ctxn]);
		if (!ctx)
			continue;

		perf_iterate_ctx(ctx, __perf_addr_filters_adjust, vma, true);
	}
	rcu_read_unlock();
}