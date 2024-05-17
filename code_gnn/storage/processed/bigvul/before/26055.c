static inline void perf_detach_cgroup(struct perf_event *event)
{
	perf_put_cgroup(event);
	event->cgrp = NULL;
}
