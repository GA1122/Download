void __perf_event_task_sched_in(struct task_struct *prev,
				struct task_struct *task)
{
	struct perf_event_context *ctx;
	int ctxn;

	 
	if (atomic_read(this_cpu_ptr(&perf_cgroup_events)))
		perf_cgroup_sched_in(prev, task);

	for_each_task_context_nr(ctxn) {
		ctx = task->perf_event_ctxp[ctxn];
		if (likely(!ctx))
			continue;

		perf_event_context_sched_in(ctx, task);
	}

	if (atomic_read(&nr_switch_events))
		perf_event_switch(task, prev, true);

	if (__this_cpu_read(perf_sched_cb_usages))
		perf_pmu_sched_task(prev, task, true);
}