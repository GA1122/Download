static inline void perf_detach_cgroup(struct perf_event *event)
{
	css_put(&event->cgrp->css);
	event->cgrp = NULL;
}
