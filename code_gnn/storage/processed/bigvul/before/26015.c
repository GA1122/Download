static inline int is_cgroup_event(struct perf_event *event)
{
	return event->cgrp != NULL;
}
