static int collect_events(struct perf_event *group, int max_count,
			  struct perf_event *event[], unsigned long *evtype,
			  int *current_idx)
{
	struct perf_event *pe;
	int n = 0;

	if (!is_software_event(group)) {
		if (n >= max_count)
			return -1;
		event[n] = group;
		evtype[n] = group->hw.event_base;
		current_idx[n++] = PMC_NO_INDEX;
	}
	list_for_each_entry(pe, &group->sibling_list, group_entry) {
		if (!is_software_event(pe) && pe->state != PERF_EVENT_STATE_OFF) {
			if (n >= max_count)
				return -1;
			event[n] = pe;
			evtype[n] = pe->hw.event_base;
			current_idx[n++] = PMC_NO_INDEX;
		}
	}
	return n;
}
