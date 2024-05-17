static int validate_group(struct perf_event *event)
{
	struct perf_event *sibling, *leader = event->group_leader;
	struct cpu_hw_events fake_cpuc;

	memset(&fake_cpuc, 0, sizeof(fake_cpuc));

	if (!validate_event(&fake_cpuc, leader))
		return -ENOSPC;

	list_for_each_entry(sibling, &leader->sibling_list, group_entry) {
		if (!validate_event(&fake_cpuc, sibling))
			return -ENOSPC;
	}

	if (!validate_event(&fake_cpuc, event))
		return -ENOSPC;

	return 0;
}
