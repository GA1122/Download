static u32 perf_event_tid(struct perf_event *event, struct task_struct *p)
{
	 
	if (event->parent)
		event = event->parent;

	return task_pid_nr_ns(p, event->ns);
}
