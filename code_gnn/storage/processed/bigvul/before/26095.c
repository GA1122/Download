static u32 perf_event_pid(struct perf_event *event, struct task_struct *p)
{
	 
	if (event->parent)
		event = event->parent;

	return task_tgid_nr_ns(p, event->ns);
}
