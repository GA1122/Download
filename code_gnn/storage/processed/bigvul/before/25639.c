static int collect_events(struct perf_event *group, int max_count,
			  struct perf_event *evts[], unsigned long *events,
			  int *current_idx)
{
	struct perf_event *event;
	int n = 0;

	if (!is_software_event(group)) {
		if (n >= max_count)
			return -1;
		evts[n] = group;
		events[n] = group->hw.event_base;
		current_idx[n++] = PIC_NO_INDEX;
	}
	list_for_each_entry(event, &group->sibling_list, group_entry) {
		if (!is_software_event(event) &&
		    event->state != PERF_EVENT_STATE_OFF) {
			if (n >= max_count)
				return -1;
			evts[n] = event;
			events[n] = event->hw.event_base;
			current_idx[n++] = PIC_NO_INDEX;
		}
	}
	return n;
}
