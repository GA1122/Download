static int group_can_go_on(struct perf_event *event,
			   struct perf_cpu_context *cpuctx,
			   int can_add_hw)
{
	 
	if (event->group_flags & PERF_GROUP_SOFTWARE)
		return 1;
	 
	if (cpuctx->exclusive)
		return 0;
	 
	if (event->attr.exclusive && cpuctx->active_oncpu)
		return 0;
	 
	return can_add_hw;
}
