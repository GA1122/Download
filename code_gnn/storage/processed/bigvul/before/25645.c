static const struct perf_event_map *niagara2_event_map(int event_id)
{
	return &niagara2_perfmon_event_map[event_id];
}
