void __perf_event_task_sched_out(struct task_struct *task,
				 struct task_struct *next)
{
	int ctxn;

	for_each_task_context_nr(ctxn)
		perf_event_context_sched_out(task, ctxn, next);

	 
	if (atomic_read(&__get_cpu_var(perf_cgroup_events)))
		perf_cgroup_sched_out(task, next);
}
