static inline void perf_get_cgroup(struct perf_event *event)
{
	css_get(&event->cgrp->css);
}
