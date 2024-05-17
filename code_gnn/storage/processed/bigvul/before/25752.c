static int collect_events(struct cpu_hw_events *cpuc, struct perf_event *leader, bool dogrp)
{
	struct perf_event *event;
	int n, max_count;

	max_count = x86_pmu.num_counters + x86_pmu.num_counters_fixed;

	 
	n = cpuc->n_events;

	if (is_x86_event(leader)) {
		if (n >= max_count)
			return -ENOSPC;
		cpuc->event_list[n] = leader;
		n++;
	}
	if (!dogrp)
		return n;

	list_for_each_entry(event, &leader->sibling_list, group_entry) {
		if (!is_x86_event(event) ||
		    event->state <= PERF_EVENT_STATE_OFF)
			continue;

		if (n >= max_count)
			return -ENOSPC;

		cpuc->event_list[n] = event;
		n++;
	}
	return n;
}