static void perf_event_free_filter(struct perf_event *event)
{
	ftrace_profile_free_filter(event);
}
