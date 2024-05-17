static int __perf_read_group_add(struct perf_event *leader,
					u64 read_format, u64 *values)
{
	struct perf_event *sub;
	int n = 1;  
	int ret;

	ret = perf_event_read(leader, true);
	if (ret)
		return ret;

	 
	if (read_format & PERF_FORMAT_TOTAL_TIME_ENABLED) {
		values[n++] += leader->total_time_enabled +
			atomic64_read(&leader->child_total_time_enabled);
	}

	if (read_format & PERF_FORMAT_TOTAL_TIME_RUNNING) {
		values[n++] += leader->total_time_running +
			atomic64_read(&leader->child_total_time_running);
	}

	 
	values[n++] += perf_event_count(leader);
	if (read_format & PERF_FORMAT_ID)
		values[n++] = primary_event_id(leader);

	list_for_each_entry(sub, &leader->sibling_list, group_entry) {
		values[n++] += perf_event_count(sub);
		if (read_format & PERF_FORMAT_ID)
			values[n++] = primary_event_id(sub);
	}

	return 0;
}