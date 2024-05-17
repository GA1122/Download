static void hw_perf_event_destroy(struct perf_event *event)
{
	perf_event_release_pmc();
}
