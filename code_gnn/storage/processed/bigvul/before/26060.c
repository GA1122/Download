static void perf_event__read_size(struct perf_event *event)
{
	int entry = sizeof(u64);  
	int size = 0;
	int nr = 1;

	if (event->attr.read_format & PERF_FORMAT_TOTAL_TIME_ENABLED)
		size += sizeof(u64);

	if (event->attr.read_format & PERF_FORMAT_TOTAL_TIME_RUNNING)
		size += sizeof(u64);

	if (event->attr.read_format & PERF_FORMAT_ID)
		entry += sizeof(u64);

	if (event->attr.read_format & PERF_FORMAT_GROUP) {
		nr += event->group_leader->nr_siblings;
		size += sizeof(u64);
	}

	size += entry * nr;
	event->read_size = size;
}